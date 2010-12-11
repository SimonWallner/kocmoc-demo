#pragma once

#include "common.hpp"
#include "Shader.hpp"


class FrameBuffer{
public:
	FrameBuffer(GLuint sizex, GLuint sizey);

	virtual ~FrameBuffer();

	/**
	* setup the shader that is used for rendering the fb quad. Override this
	* method in subclasses!
	*/
	virtual void setupShader(void);

	void setupBaseShader();

	GLuint getTextureHandle();
	GLuint getFBOHandle();
	virtual void drawFBO();

	virtual void drawFBO(GLfloat topX, GLfloat topY, GLfloat bottomX, GLfloat bottomY);

private:

	bool depth;
	
	GLuint textureHandle;
	GLuint depthbufferHandle; 

protected:
	FrameBuffer(void);

	GLuint fboHandle;

	GLuint FBOSizeX;
	GLuint FBOSizeY;

	Shader *shader;
	virtual void setFBOTexture();

};