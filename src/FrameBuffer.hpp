#ifndef FRAMEBUFFER_HPP_
#define FRAMEBUFFER_HPP_

#include "common.hpp"
#include "Shader.hpp"

namespace kocmoc
{

	class FrameBuffer
	{
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

	private:
	
		GLuint textureHandle;
		GLuint depthbufferHandle; 
	
		Shader *shader;

		FrameBuffer(void);

		GLuint fboHandle;

		GLuint FBOSizeX;
		GLuint FBOSizeY;

		GLuint *vbo_id;
		GLuint vao_id;
	
		void setFBOTexture();
		void createQuad();
		void check();


	};
}

#endif
