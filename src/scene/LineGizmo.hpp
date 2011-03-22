#ifndef KOCMOC_SCENE_LINE_GIZMO_HPP_
#define KOCMOC_SCENE_LINE_GIZMO_HPP_

#include <common.hpp>

namespace kocmoc
{
	namespace renderer
	{
		class Shader;
	}

	namespace camera
	{
		class Camera;
	}

	namespace scene
	{
		class LineGizmo
		{
		public:

			/**
			 * Construct a new Line Gizmo from the given data and upload it 
			 * to the GPU;
			 * @param vertexPositions the interleaved (x, y, z) vertex position
			 * data
			 * 
			 * @param length The length of the data array above
			 * @param indices the index data
			 * @param indexCount the length of the indices array
			 */
			LineGizmo(float* vertexPositions, float* vertexColors, uint length,
				uint* indices, uint indexCount);

			~LineGizmo(void);

			void draw(glm::mat4 leaftTransform, camera::Camera *camera);
		private:
			GLuint* vboHandles;
			GLuint vaoHandle;
			renderer::Shader* shader;
			uint indexCount;
		};
	}
}


#endif