#ifndef KOCMOC_SCENE_POLY_MESH_HPP_
#define KOCMOC_SCENE_POLY_MESH_HPP_

#include "meshUtils.hpp"

#include <renderer/RenderMesh.hpp>
#include <Symbol.hpp>

#include <map>

namespace kocmoc
{
	namespace renderer
	{
		class RenderMesh;
	}

	namespace scene
	{
		/**
		 * The \c PolyMesh acts as the canonical basis for polygonal meshes.
		 *
		 * It consists of convex primitives and uses an arbitrary number of indexed
		 * vertex Attributes (\see vertexAttribute), vertex positions must be provided 
		 * in the construction step however.
		 *
		 * Vertex attributes are handled by a unique \c Symbol
		 *
		 * @note	only primitives with at least 3 vertices are supported so far.
		 *			That means no line or point primitives. Primitives with more than
		 *			3 vertices are triangulated before they are sent to the GPU.
		 */

		// TODO textures
		// TODO make split a friend utility function
		// TODO externalize all function into other classes, keep dependencies to a minimum
		//		remove draw/render methods

		class PolyMesh
		{
			friend class renderer::RenderMesh;
			friend SplitResult splitMesh();

		public:

			

			/**
			 * The vertex attribute struct.
			 * It holds together all the data needed for such an attribute.
			 *
			 * @note	All fields in this structure are const. This should be nice
			 *			when it comes to data encapsulation and such, however
			 *			it prevents the compiler from creating a default \c operator=()
			 *			This means that it can not be used like map[symbol] = bar, instead
			 *			the explicit way over pair<> and insert() has to be taken.
			 */
			struct VertexAttribute
			{
				/** the stride of the attribute. E.g stride=3 for a 3d position
				 * and stride=2 for UV coordinates */
				const uint stride;

				/** The length of the \c attributeData array.
				* E.g for a constant colored mesh it would be 4 (1xRGBA). */
				const uint attributeDataLength;

				/** The attribute data array */
				const double* attributeData;

				/** Indices for this attribute. Every attribute can have its own
				 * indices. Its length must match the mesh' \c vertexCount */
				const uint* indices;

				/** whether or whether not we own the data, i.e. are allowed to delete it */
				const bool hasOwnership;

				/**
				 * construct a new vertexAttribute instance from the given data.
				 * Parameter semantics are as the fields described above
				 */
				VertexAttribute(uint _stride, uint _attributeDataLength,
					double* _attributeData, uint* _indices, bool handOverOwnership)
					: stride(_stride)
					, attributeDataLength(_attributeDataLength)
					, attributeData(_attributeData)
					, indices(_indices)
					, hasOwnership(handOverOwnership)
				{};

			};

			/**
			 * Simple structure to define a bounding box.
			 * The 3d min and max coordinates define a axis aligned cube in space
			 */
			struct BoundingBox
			{
				glm::dvec3 min;
				glm::dvec3 max;
			};
				
			typedef std::map<Symbol, VertexAttribute> VertexAttributeMap;
			typedef std::pair<Symbol, VertexAttribute> vertexAttributePair;

			/**
			 * The number of non-unique vertices in this mesh.
			 * 
			 * A cube for instance has a \c vertexIndecCount of 24 (4 sides * 4
			 * vertices per plane). However This cube would only have 8 unique
			 * vertex positions. 
			 *
			 * @note	the \c vertexIndexCount defines the length of the vertex index
			 *			arrays and it is constant.
			 */
			const uint vertexIndexCount;

			/**
			 * The number of primitives in this mesh.
			 *
			 * A primitive can be a point, a line or a polygon.
			 *
			 * @note	only polygon primitives are supported so far
			 */
			const uint primitiveCount;

			/**
			 * This array points to the first indices of each primitive. It points 
			 * into the index arrays of the vertex attributes.
			 *
			 * @note	its length must be 1 grater than primitiveCount. The last entry
			 *			points beyond the end of the indices array. It is however
			 *			necessary to compute the length of a primitive by subtracting
			 *			consecutive indices like fia[i+1] - fia[i].
			 */
			const uint* firstIndexArray;

			/**
			 * Create a new \c PolyMesh.
			 * 
			 * @param	primitiveCount	the number of primitives (polygons, etc...)
			 * @param	vertexIndexCount	the number of vertex indices
			 * @param	firstIndexArray		The array that points to the first index of every primitive
			 * @param	vertexAttributes	the vertex positions
			 */
			PolyMesh(uint primitiveCount, uint vertexIndexCount,
				uint* firstIndexArray, VertexAttribute vertexPositions);

			/**
			 * deconstruct the mesh and delete the data fields it owns.
			 */
			virtual ~PolyMesh();

			/**
			 * Add a vertex Attribute.
			 *
			 * An arbitrary number of vertex attributes can be added to a mesh
			 *
			 * @param name the name of the attribute under which it will be filed
			 * @param attribute the vertex attribute itself
			 *
			 * @note	the given vertex attribute must conform to the mesh structure
			 *			i.e. the vertex index count must match.
			 */
			void addVertexAttribute(Symbol name, VertexAttribute attribute);

			/**
			 * Calculate the bounding box of this mesh
			 */
			BoundingBox calculateBoundingBox(void) const;
		
			///**
			// * Draw the mesh.
			// * If the data is not on the GPU yet it will be transferred upon the first call
			// */
			//void draw(glm::mat4 parentTransform, camera::Camera *camera, renderer::Shader *shader = NULL);

			//void setShader(renderer::Shader *shader);

			//void setDiffuseTexture(GLint textureHandle);
			//void setSpecularTexture(GLint textureHandle);
			//void setNormalTexture(GLint textureHandle);


			//unsigned int getPrimitiveCount(void) {return primitiveCount;};
			//unsigned int getVertexIndexCount(void) {return vertexIndexCount;};
			//unsigned int getVertexCount(void) {return vertexIndexCount;};

			//void setModelMatrix(glm::mat4 modelMatrix);
			//glm::mat4 getModelMatrix(void) {return modelMatrix;};

			///**
			// * Split the mesh at the given splitPlane
			// * @param d the normal distance origin-plane
			// * @param n the plane normal, it is assumed to point
			// * to the 'outside'
			// */
			//SplitResult split(double d, glm::vec3 n);

		private:

			/** The map of vertex attributes */
			VertexAttributeMap vertexAttributes;



			//**
			// * Transfer the data to the GPU, i.e. write the vbos, etc...
			// */
			//// create and organize renderMesh thingies
			//void transferData();


			//**
			// * number of vertex indices after triangulation
			// * this is used for rendering in the drawElements call
			// */
			//unsigned int triangulatedVertexIndexCount;

			//unsigned int *firstIndexArray;
			//unsigned int *vertexIndexArray;
			//unsigned int *uvIndexArray;
			//unsigned int *normalIndexArray;

			//double *vertexPositions;
			//double *uvPositions;
			//double *normalPositions;
		
			//glm::mat4 modelMatrix;
			////glm::mat3 normalMatrix;

			//** the GPU view of the polyMesh **/
			////RenderMesh *renderMesh;

			//// TODO: refactor into render mesh class
			//// make a poly Mesh hold a render mesh for rendering
			//GLuint vaoHandle, *vboHandles;

			//GLuint diffuseTextureHandle;
			//GLuint specularTextureHandle;
			//GLuint normalTextureHandle;

			//renderer::Shader *shader;

			//bool dataIsUploaded;
			
		};
	}
}

#endif