#ifndef KOCMOC_SCENE_MESH_UTILS_HPP
#define KOCMOC_SCENE_MESH_UTILS_HPP

#include <types.hpp>
#include <Symbol.hpp>
#include <scene/PolyMesh.hpp>

#include <glm.hpp>

#include <map>
#include <vector>

namespace kocmoc
{
	namespace scene
	{
		namespace meshUtils
		{
			typedef std::map<uint, uint > IndexMap;

			struct SplitResult
			{
				PolyMesh* inside;
				PolyMesh* outside;
			};

			struct AttributeBuffer
			{
				/** a map pointing from the old indices to the new indices */
				IndexMap indexMap;

				/** the new indices pointing into the data array*/
				std::vector<uint > indices;

				/** the next index to be inserted as a new index */
				uint nextIndex;

				/** the new attribute data */
				std::vector<double > data;

				AttributeBuffer()
					: nextIndex(0)
				{};

				// TODO: add vertexCount alias to nextIndex
			};

			typedef std::map<Symbol, AttributeBuffer > AttributeBufferMap;


			/**
			 * Split the mesh at the given splitPlane
			 * @param d the normal distance origin-plane
			 * @param n the plane normal, it is assumed to point
			 * to the 'outside'
			 */
			SplitResult splitMesh(const PolyMesh* mesh, const double d, const glm::dvec3 n);

			/**
			 * Transfer attributes, batch mode!
			 */
			void transferAttributes(AttributeBufferMap& targetBufferMap,
				const scene::PolyMesh::VertexAttributeMap& vertexAttributes,
				const uint currentIndex);

			/**
			 * batch lerp and transfer data
			 */
			void lerpTransferAttributes(AttributeBufferMap& insideTargetBufferMap,
				AttributeBufferMap& outsideTargetBufferMap,
				const scene::PolyMesh::VertexAttributeMap& vertexAttributes,
				const uint currentIndex,
				const uint nextIndex,
				const double r);

		}
	}
}

#endif