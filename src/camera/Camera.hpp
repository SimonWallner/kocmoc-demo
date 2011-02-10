#ifndef KOCMOC_CAMERA_CAMERA_H_
#define KOCMOC_CAMERA_CAMERA_H_

#include <common.hpp>

namespace kocmoc
{
	namespace renderer
	{
		class Shader;
	}

	namespace camera
	{

		/**
		 * Abstract camera class
		 */
		class Camera
		{
		public:

			/** 
			 * Get the view matrix of this camera
			 * @return the view matrix of the camera
			 */
			virtual glm::mat4 getViewMatrix() = 0;

			/**
			 * Get the projection matrix of this camera
			 */
			virtual glm::mat4 getProjectionMatrix() = 0;

			/**
			 * Get the untranslated view matrix that only reflects the viewing
			 * direction. made for the skybox.
			 * @return the untranslated view matrix
			 */
			virtual glm::mat4 getUntraslatedViewMatrix() = 0;

			virtual glm::vec3 getPosition(void) {return glm::vec3(0.0f);};

			/**
			 * Update the view matrix of this camera with the current values. 
			 * You should call this method at the beginning of every frame. Camera
			 * parameters (eyePsition, targetPosition, etc.) my change during a 
			 * frame due to user inputs, but the viewMatrix remains constant until
			 * <code>updateViewMatrix()</code> is called.
			 * Also updates the untranslated view Matrix.
			 */
			virtual void updateMatrixes() = 0;

			void drawGizmo();
			void setupGizmo(void);

		private:
			GLuint *gizmo_vbos, gizmo_vao;
			renderer::Shader *gizmoShader;
		};
	}
}
#endif