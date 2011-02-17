#ifndef KOCMOC_RENDERER_RENDER_MESH_HPP_
#define KOCMOC_RENDERER_RENDER_MESH_HPP_

#include "Shader.hpp"
#include <scene/PolyMesh.hpp>

#include <glm.hpp>

namespace kocmoc
{
	namespace scene
	{
		class PolyMesh;
	}

	namespace camera
	{
		class Camera;
	}

	namespace renderer
	{
		/**
		 * Wrap a polyMesh so that we can enhance it with further properties
		 * and finally render it.
		 *
		 * Practically a RenderMesh is friends with a PolyMesh in order to ease
		 * cooperation. This is indeed tight coupling, but it seems appropriate
		 * here.
		 */
		class RenderMesh
		{
		public:
			const scene::PolyMesh* mesh;
			const Shader* shader;

			RenderMesh(scene::PolyMesh* mesh, Shader* shader);

			void draw(glm::mat4 parentTransform, camera::Camera *camera);

			void setModelMatrix(glm::mat4 _modelMatrix);

		private:
			glm::mat4 modelMatrix;

			bool isUploaded;

			void uploadData(void);
			GLuint vaoHandle;
			uint triangulatedVertexIndexCount;
		};
	}
}



#endif