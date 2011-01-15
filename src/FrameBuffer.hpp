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

		//void setupBaseShader();

		GLuint getTextureHandle();
		GLuint getFBOHandle();
		virtual void drawFBO();

		void toggleColorCorrection(void);
		void toggleProjection(void);
		void toggleVignetting(void);

	private:
	
		GLuint textureHandle;
		GLuint depthbufferHandle; 
		GLint colorLUTHandle;

		Shader *shader;

		FrameBuffer(void);

		GLuint fboHandle;

		GLuint width;
		GLuint height;

		GLuint *vbo_id;
		GLuint vao_id;
	
		void setFBOTexture();
		void createQuad();
		void check();

		bool enableColorCorrection;
		bool enableNonPlanarProjection;
		bool enableVignetting;
	};
}

#endif
