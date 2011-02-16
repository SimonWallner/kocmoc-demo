#ifndef KOCMOC_SCENE_POLY_MESH_HPP_
#define KOCMOC_SCENE_POLY_MESH_HPP_

#include "LineGizmo.hpp"

#include <renderer/Shader.hpp>
#include <camera/Camera.hpp>
#include <Symbol.hpp>

#include <map>

namespace kocmoc
{
	namespace scene
	{
		/**
		 * The \c PolyMesh acts as the canonical basis for polygonal meshes.
		 *
		 * A \c polyMesh can hold any number of \b convex polygons. Polygons
		 * consist of \b vertices that have a position and an arbitrary number
		 * of named \b vertex attributes like UV-position, color, normal, etc...
		 * 
		 * @par It is structured as follows:
		 * At the heart of the \c PolyMesh is the \c firstIndexArray. It holds 
		 * the indices of the \c vertxIndexArray where a polygon starts.
		 */


		class PolyMesh
		{
		public:

			struct SplitResult
			{
				PolyMesh* inside;
				PolyMesh* outside;
			};

			/**
			 * The vertex attribute struct.
			 * It holds together all the data needed for such an attribute.
			 */
			struct vertexAttribute
			{
				/** the stride of the attribute. E.g stride=3 for a 3d position
				 * and stride=2 for UV coordinates */
				uint stride;

				/** The number of attributes. I.e the length of the \c attributeData
				 * array. E.g for a constant colored mesh it would be 4 (1xRGBA). */
				uint attributeDataLength;

				/** The attribute data array */
				double* attributeData;

				/** Indices for this attribute. Every attribute can have its own
				 * indices. Its length must match the mesh' \c vertexCount */
				uint* indices;
			};
				
			typedef std::map<Symbol, vertexAttribute> VertexAttributeMap;

			/** The number of vertices in this mesh.
			 * A cube for instance has a vertexCount of 24 (4 sides * 4 vertices 
			 * per plane). However This cube would only have 8 unique vertex
			 * positions. 
			 *
			 * @note the \c vertexCount defines the length of the vertex index
			 * arrays and it is constant
			 */
			const uint vertexCount;

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
			 * If the data is not on the GPU yet it will be transferred upon the first call
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

			/**
			 * Split the mesh at the given splitPlane
			 * @param d the normal distance origin-plane
			 * @param n the plane normal, it is assumed to point
			 * to the 'outside'
			 */
			SplitResult split(double d, glm::vec3 n);

		private:

			/** The map of vertex attributes */
			VertexAttributeMap vertexAttributes;



			/**
			 * Transfer the data to the GPU, i.e. write the vbos, etc...
			 */
			// create and organize renderMesh thingies
			void transferData();


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
			
			LineGizmo* gizmo;
		};
	}
}

#endif