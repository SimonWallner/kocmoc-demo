#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer(GLuint sizex, GLuint sizey): FBOSizeX(sizex), FBOSizeY(sizey)
{
	// generate fbo
	glGenFramebuffers(1, &fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

	// create a depth-buffer
	glGenRenderbuffers(1, &depthbufferHandle);
	// bind our depth-buffer
	glBindRenderbuffer(GL_RENDERBUFFER, depthbufferHandle);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, sizex, sizey);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbufferHandle);

	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, sizex, sizey, 0, GL_RGBA, GL_FLOAT, NULL);

	get_errors();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureHandle, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		switch(status) {
			case GL_FRAMEBUFFER_UNSUPPORTED:
				cout<<"Unsupported framebuffer format"<<endl;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				cout<<"Framebuffer incomplete, missing attachment"<<endl;
				break;
			//case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
			//	cout<<"Framebuffer incomplete, attached images must have same dimensions"<<endl;
			//	break;
			//case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
			//	cout<<"Framebuffer incomplete, attached images must have same format"<<endl;
			//	break;
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::FrameBuffer()
{
}


void FrameBuffer::setupShader()
{
	cout << "Frame buffer setup shader" << endl;
	shader = new Shader("post.vert", "post.frag");
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
	drawFBO(-1, 1, 1, -1);
}

void FrameBuffer::drawFBO(GLfloat topX, GLfloat topY, GLfloat bottomX, GLfloat bottomY)
{
	shader->bind();

	setFBOTexture();

	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_UV0);

	GLfloat quadVertices[] = {bottomX,  bottomY, 0.0f,
		bottomX, topY, 0.0f,
		topX, topY, 0.0f,
		topX, bottomY, 0.0f};

	GLfloat quadTexCoord[] = {1.0f, 0.0f,
		1.0f,  1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f };

	glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, quadVertices);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_UV0, 2, GL_FLOAT, GL_FALSE, 0, quadTexCoord);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	// Clean up
	glDisableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);
	glDisableVertexAttribArray(VERTEX_ATTR_INDEX_UV0);
	glBindTexture(GL_TEXTURE_2D, 0);
	shader->unbind();
}


void FrameBuffer::setFBOTexture(){
	if(textureHandle != 0)
	{
		GLint sTex0_location = shader->get_uniform_location("sTex0");
		
		if (sTex0_location >= 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureHandle);
			
			glGenerateMipmap(GL_TEXTURE_2D);

			glUniform1i(sTex0_location, 0);
		} else 
		{
			cout << "failed to get retrieve the uniform position for sTex0" << endl;
			cout << "uniform locatio is: " << sTex0_location << endl;
		}
	}
}