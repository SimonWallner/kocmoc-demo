#ifndef KOCMOC_SCENE_POLY_MESH_HPP_
#define KOCMOC_SCENE_POLY_MESH_HPP_

#include <renderer/Shader.hpp>
#include <camera/Camera.hpp>

namespace kocmoc
{
	namespace scene
	{
		/**
		 * The PolyMesh acts as the cannonical basis for meshes in this application.
		 *
		 * Data that is generated or imported is transfered into this internal
		 * representation. It preserves the topological information on a low level basis.
		 * That is, primitives with vertex indices in a set of shared vertices. Adjacency
		 * information is not directly available but could be computed from the data.
		 *
		 * Data is organized in primitives (points, lines, convex n-gons), vertex indices
		 * of this primitives and shared vertex attributes (position, etc...).
		 */

		// TODO: additional vertex attributes
		// TODO: Face attributes
		// TODO: Face-Vertex attributes

		class PolyMesh
		{
		public:
			/**
			 * Create a new poly mesh. Counts must be known in advance to avoid 
			 * reallocation of arrays and such
			 */
			PolyMesh(unsigned int primitiveCount,
				unsigned int vertexIndexCount,
				unsigned int vertexCount);

			virtual ~PolyMesh();


			// set fia, indices and positions
			void setFirstIndexArray(unsigned int *fia);

			void setVertexIndexArray(unsigned int *vertexIndices);
			void setUVIndexArray(unsigned int *uvIndices);
			void setNormalIndexArray(unsigned int *normalIndices);

			void setVertexPositions(double *positions);
			void setUVPositions(double *uv);
			void setNormalPositions(double *normals);

			


			/**
			 * Draw the mesh.
			 * If the data is not on the gpu yet it will be transfered upon the first call
			 */
			void draw(glm::mat4 parentTransform, camera::Camera *camera, renderer::Shader *shader = NULL);

			void setShader(renderer::Shader *shader);

			void setDiffuseTexture(GLint textureHandle);
			void setSpecularTexture(GLint textureHandle);
			void setNormalTexture(GLint textureHandle);

			// TODO: recompute face normals

			unsigned int getPrimitiveCount(void) {return primitiveCount;};
			unsigned int getVertexIndexCount(void) {return vertexIndexCount;};
			unsigned int getVertexCount(void) {return vertexCount;};

			void setModelMatrix(glm::mat4 modelMatrix);
			glm::mat4 getModelMatrix(void) {return modelMatrix;};

		private:

			/**
			 * Transfer the data to the gpu, i.e. write the vbos, etc...
			 */
			// create and organize renderMesh thingys
			void transferData();

			/** total number of vertices **/
			unsigned int vertexCount;

			/** number of vertex indices **/
			unsigned int vertexIndexCount;

			/**
			 * number of vertex indices after triangulation
			 * this is used for rendering in the drawElements call
			 */
			unsigned int triangulatedVertexIndexCount;

			/** number of primitives **/
			unsigned int primitiveCount;

			unsigned int *firstIndexArray;
			unsigned int *vertexIndexArray;
			unsigned int *uvIndexArray;
			unsigned int *normalIndexArray;

			double *vertexPositions;
			double *uvPositions;
			double *normalPositions;
		
			glm::mat4 modelMatrix;
			//glm::mat3 normalMatrix;

			/** the GPU view of the polyMesh **/
			//RenderMesh *renderMesh;

			// TODO: refactor into render mesh class
			// make a poly Mesh hold a render mesh for rendering
			GLuint vaoHandle, *vboHandles;

			GLuint diffuseTextureHandle;
			GLuint specularTextureHandle;
			GLuint normalTextureHandle;

			renderer::Shader *shader;

			bool dataIsUploaded;
		};
	}
}

#endif