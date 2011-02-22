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
			const scene::PolyMesh* mesh;
			Shader* shader;

			RenderMesh(scene::PolyMesh* mesh, Shader* shader);

			void draw(glm::mat4 parentTransform, camera::Camera *camera);

			void setModelMatrix(glm::mat4 _modelMatrix);

		private:

			/** A texture wrapper */
			struct RenderTexture
			{
				const Symbol name;
				const scene::PolyMesh::Texture texture;
				const GLuint handle;
				const GLuint textureUnit;

				RenderTexture(Symbol _name, scene::PolyMesh::Texture _texture, GLuint _handle, GLuint _textureUnit)
					: name(_name)
					, texture(_texture)
					, handle(_handle)
					, textureUnit(_textureUnit)
				{};
			};

			typedef std::vector<RenderTexture > TextureList;

			TextureList textures;

			glm::mat4 modelMatrix;

			bool isUploaded;

			void uploadData(void);

			/**
			 * upload the shader and set params
			 */
			void setUpShader(void);

			GLuint vaoHandle;
			uint triangulatedVertexIndexCount;

			scene::LineGizmo* originGizmo;
			scene::LineGizmo* boundingBox;

			glm::mat4 bbTransform;
		};
	}
}



#endif