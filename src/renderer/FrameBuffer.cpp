#include "FrameBuffer.hpp"

#include "ShaderManager.hpp"

#include <Context.hpp>
#include <Kocmoc.hpp>
#include <time/Clock.hpp>
#include <loader/ImageLoader.hpp>
#include <util/Property.hpp>
#include <util/util.hpp>
#include <camera/FilmCamera.hpp>

using namespace kocmoc::renderer;

using kocmoc::loader::ImageLoader;

using std::cout;
using std::endl;
using kocmoc::util::math::sign;
using kocmoc::util::math::min;

FrameBuffer::FrameBuffer(int _frameWidth, int _frameHeight, int _gateWidth, int _gateHeight)
	: frameWidth(_frameWidth * 2)
	, frameHeight(_frameHeight * 2)
	, gateWidth(_gateWidth * 2)
	, gateHeight(_gateHeight * 2)
	, enableColorCorrection(util::Property("enableColorCorrection"))
	, enableNonPlanarProjection(util::Property("enableNonPlanarProjection"))
	, enableVignetting(util::Property("enableVignetting"))
	, logAdaptationPerS(util::Property("logAdaptationPerS"))
	, evBias(util::Property("evBias"))
	, currentAdaptation(0.0f)
{
	// generate fbo
	glGenFramebuffers(1, &fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

	// create a depth-buffer
	glGenRenderbuffers(1, &depthbufferHandle);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbufferHandle);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, frameWidth, frameHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbufferHandle);

	// create and bind texture
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, frameWidth, frameHeight, 0, GL_RGBA, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureHandle, 0);


	// create and bind log Y texture
	glGenTextures(1, &logYTextureHandle);
	glBindTexture(GL_TEXTURE_2D, logYTextureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, frameWidth, frameHeight, 0, GL_RED, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, logYTextureHandle, 0);


	check();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	createQuad();

	colorLUTHandle = ImageLoader::getInstance().loadImage3D("LUT32.png", false);

	maxMipLevel = uint(log(float(((frameWidth > frameHeight) ? frameWidth : frameHeight))) / log(2.0f));
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
	// TODO: migrate to renderMesh

	GLfloat topX = -(float)(gateWidth / 2) / Context::getInstance().width;
	GLfloat topY = (float)(gateHeight / 2) / Context::getInstance().height;
	GLfloat bottomX = (float)(gateWidth / 2) / Context::getInstance().width;
	GLfloat bottomY = -(float)(gateHeight / 2) / Context::getInstance().height;

	float horizontalScale = (float)gateWidth / frameWidth;
	float horizontalOffset = (1 - horizontalScale) / 2.0f;
	float verticalScale = (float)gateHeight / frameHeight;
	float verticallOffset = (1 - verticalScale) / 2.0f;

	GLfloat quadVertices[] = {bottomX,  bottomY, 0.0f,
		bottomX, topY, 0.0f,
		topX, topY, 0.0f,
		topX, bottomY, 0.0f};

	GLfloat quadTexCoord[] = {	horizontalScale + horizontalOffset,	verticallOffset,
								horizontalScale + horizontalOffset,	verticalScale + verticallOffset,
								horizontalOffset,					verticalScale + verticallOffset,
								horizontalOffset,					verticallOffset};

	vbo_id = new GLuint[2];
	glGenBuffers(2, vbo_id);
	glGenVertexArrays(1, &vao_id);

	glBindVertexArray(vao_id);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 *sizeof(float), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), quadTexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void FrameBuffer::setupShader()
{
	shader = ShaderManager::getInstance().load("post.vert", "post.frag");
	shader->addSemantic(Shader::VertexAttributeSemantic(symbolize("position"),
					VERTEX_ATTR_NAME_POSITION, 0));
	shader->addSemantic(Shader::VertexAttributeSemantic(symbolize("uv0"),
					VERTEX_ATTR_NAME_UV0, 1));

	shader->upload();

	shader->bind();

	GLint location;
	if ((location = shader->get_uniform_location(DIFFUSE_SAMPLER_NAME)) > 0)
		glUniform1i(location, 0);
	if ((location = shader->get_uniform_location(SPECULAR_SAMPLER_NAME)) > 0)
		glUniform1i(location, 1);
	if ((location = shader->get_uniform_location(NORMAL_SAMPLER_NAME)) > 0)
		glUniform1i(location, 2);
	if ((location = shader->get_uniform_location(SHADOW_SAMPLER_NAME)) > 0)
		glUniform1i(location, 3);

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
			glUniform2i(location, gateWidth, gateHeight);

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

GLuint FrameBuffer::getLogYHandle()
{
	return logYTextureHandle;
}

GLuint FrameBuffer::getFBOHandle()
{
	return fboHandle;
}

void FrameBuffer::drawFBO()
{
	shader->bind();
	setFBOTexture();

	
	// read average log Y from texture
	glBindTexture(GL_TEXTURE_2D, logYTextureHandle);
	glGenerateMipmap(GL_TEXTURE_2D);
		
	GLfloat average;
	glGetTexImage(GL_TEXTURE_2D, maxMipLevel, GL_RED, GL_FLOAT, &average);

	float dLum = average - currentAdaptation; // increase --> positive dLum
	int s = sign(dLum);
	float a = abs(dLum);
	float x = min(1.0f, 2.0f);
	float d = Kocmoc::getInstance().getClock()->lastFrameDuration();

	float change = sign(dLum) * min<float>(logAdaptationPerS * Kocmoc::getInstance().getClock()->lastFrameDuration(), abs(dLum));
	currentAdaptation += change;

	GLuint location;
	if ((location = shader->get_uniform_location("evBias")) >= 0)
			glUniform1f(location, evBias);

	if ((location = shader->get_uniform_location("logLuminance")) >= 0)
		glUniform1f(location, currentAdaptation);
	


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
		//glGenerateMipmap(GL_TEXTURE_2D);

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
	std::cout << "non-planar projection is: " << enableNonPlanarProjection << std::endl;
}

void FrameBuffer::toggleVignetting()
{
	enableVignetting = !enableVignetting;
	std::cout << "vignetting is: " << enableVignetting << std::endl;
}