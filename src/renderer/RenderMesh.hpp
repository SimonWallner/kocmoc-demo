#ifndef KOCMOC_RENDERER_RENDER_MESH_HPP_
#define KOCMOC_RENDERER_RENDER_MESH_HPP_

#include "Shader.hpp"

#include <scene/PolyMesh.hpp>

#include <glm.hpp>

#include <vector>

namespace kocmoc
{
	namespace scene
	{
		class LineGizmo;
	}

	namespace camera
	{
		class Camera;
	}

	namespace renderer
	{
		// TODO add shader attribute semantic hint/explanation/ref

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
			// TODO: rename to polyMesh
			const scene::PolyMesh* mesh;
			Shader* shader;

			RenderMesh(scene::PolyMesh* mesh, Shader* shader);
			~RenderMesh(void);

			void draw(glm::mat4 parentTransform, camera::Camera *camera);

			void setModelMatrix(glm::mat4 _modelMatrix);

			uint getVertexCount(void);

		private:

			/** A texture wrapper */
			struct RenderTexture
			{
				const GLuint handle;
				const GLuint textureUnit;

				RenderTexture(GLuint _handle, GLuint _textureUnit)
					: handle(_handle)
					, textureUnit(_textureUnit)
				{};
			};

			typedef std::vector<RenderTexture > RenderTextureList;

			RenderTextureList renderTextures;

			glm::mat4 modelMatrix;

			bool isUploaded;

			void uploadData(void);

			/** Set up the textures.
			 * 
			 * - read bindings from shader
			 * - fetch specified textures from poly mesh
			 * - load these textures 
			 * - store in render textures
			 */
			void setUpTextures(void);

			GLuint vaoHandle;
			uint triangulatedVertexIndexCount;

			scene::LineGizmo* originGizmo;
			scene::LineGizmo* boundingBox;

			glm::mat4 bbTransform;

			// uniform locations
			GLint uniformCameraPosition;
			GLint uniformProjectionMatrix;
			GLint uniformViewMatrix;
			GLint uniformModelMatrix;
			GLint uniformNormalMatrix;
			bool uniformsAreSet;

			bool debugDrawMeshGizmo;

		};
	}
}



#endif