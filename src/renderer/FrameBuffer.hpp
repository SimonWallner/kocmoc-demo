#ifndef FRAMEBUFFER_HPP_
#define FRAMEBUFFER_HPP_

#include "common.hpp"
#include "Shader.hpp"

namespace kocmoc
{

	class FrameBuffer
	{
	public:
		FrameBuffer(int frameWidth, int frameHeight, int gateWidth, int gateHeight);

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

		const int gateWidth;
		const int gateHeight;
		const int frameWidth;
		const int frameHeight;

	private:
	
		GLuint textureHandle;
		GLuint depthbufferHandle; 
		GLint colorLUTHandle;

		Shader *shader;

		FrameBuffer(void);

		GLuint fboHandle;

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
