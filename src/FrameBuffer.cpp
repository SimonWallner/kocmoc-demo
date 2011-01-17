#include "FrameBuffer.hpp"
#include "ShaderManager.hpp"
#include "ImageLoader.hpp"
#include "Property.hpp"
#include "Context.hpp"
#include "Kocmoc.hpp"
#include "FilmCamera.hpp"

using namespace kocmoc;

FrameBuffer::FrameBuffer(GLuint sizex, GLuint sizey)
	: width(sizex * 2)
	, height(sizey * 2)
	, enableColorCorrection(util::Property("enableColorCorrection"))
	, enableNonPlanarProjection(util::Property("enableNonPlanarProjection"))
	, enableVignetting(util::Property("enableVignetting"))
{
	// generate fbo
	glGenFramebuffers(1, &fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

	// create a depth-buffer
	glGenRenderbuffers(1, &depthbufferHandle);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbufferHandle);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbufferHandle);

	// create and bind texture
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureHandle, 0);

	check();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	createQuad();

	colorLUTHandle = ImageLoader::getInstance().loadImage3D("LUT32.png", false);
}

void FrameBuffer::check()
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		switch(status) {
			case GL_FRAMEBUFFER_UNSUPPORTED:
				cout<<"Unsupported framebuffer format"<<endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				cout<<"Framebuffer incomplete, missing attachment"<<endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				cout<<"Framebuffer incomplete, missing draw buffer"<<endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				cout<<"Framebuffer incomplete, missing read buffer"<<endl;
				break;
			default:
				cout<<"Unknown Framebuffer error"<<endl;
				break;
		}
	}
	else{
		cout<<"done"<<endl;
	}
}
void FrameBuffer::createQuad()
{
	GLfloat topX = -(float)(width / 2) / Context::getInstance().width;
	GLfloat topY = (float)(height / 2) / Context::getInstance().height;
	GLfloat bottomX = (float)(width / 2) / Context::getInstance().width;
	GLfloat bottomY = -(float)(height / 2) / Context::getInstance().height;

	GLfloat quadVertices[] = {bottomX,  bottomY, 0.0f,
		bottomX, topY, 0.0f,
		topX, topY, 0.0f,
		topX, bottomY, 0.0f};

	GLfloat quadTexCoord[] = {1.0f, 0.0f,
		1.0f,  1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f };

	vbo_id = new GLuint[2];
	glGenBuffers(2, vbo_id);
	glGenVertexArrays(1, &vao_id);

	glBindVertexArray(vao_id);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 *sizeof(float), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), quadTexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_UV0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_UV0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void FrameBuffer::setupShader()
{
	shader = ShaderManager::getInstance().load("post.vert", "post.frag");

	shader->bind();
	{
		GLint location;
		
		if ((location = shader->get_uniform_location("sTex0")) >= 0)
			glUniform1i(location, 0);

		if ((location = shader->get_uniform_location("sColorLUT")) >= 0)
			glUniform1i(location, 1);

		if ((location = shader->get_uniform_location("nonPlanarProjection")) >= 0)
			glUniform1i(location, enableNonPlanarProjection);
		
		if ((location = shader->get_uniform_location("colorCorrection")) >= 0)
			glUniform1i(location, enableColorCorrection);

		if ((location = shader->get_uniform_location("vignetting")) >= 0)
			glUniform1i(location, enableVignetting);

		if ((location = shader->get_uniform_location("dimension")) >= 0)
			glUniform2i(location, width, height);

		if ((location = shader->get_uniform_location("angleOfView")) >= 0)
			glUniform1f(location, Kocmoc::getInstance().getCamera()->getAngleOfView());
	}
	shader->unbind();
}

FrameBuffer::~FrameBuffer()
{
}

GLuint FrameBuffer::getTextureHandle()
{
	return textureHandle;
}

GLuint FrameBuffer::getFBOHandle()
{
	return fboHandle;
}

void FrameBuffer::drawFBO()
{
	shader->bind();
	setFBOTexture();
	
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
	
	shader->unbind();
}


void FrameBuffer::setFBOTexture()
{
	if(textureHandle != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glGenerateMipmap(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_3D, colorLUTHandle);
	}
}


void FrameBuffer::toggleColorCorrection()
{
	enableColorCorrection = !enableColorCorrection;
	std::cout << "color correction is: " << enableColorCorrection << std::endl;
}

void FrameBuffer::toggleProjection()
{
	enableNonPlanarProjection = !enableNonPlanarProjection;
	std::cout << "non planar projection is: " << enableNonPlanarProjection << std::endl;
}

void FrameBuffer::toggleVignetting()
{
	enableVignetting = !enableVignetting;
	std::cout << "vignetting is: " << enableVignetting << std::endl;
}