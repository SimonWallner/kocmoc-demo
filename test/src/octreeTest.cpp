/**
 * ocrteeTest
 * basic test file for the ocree implementation
 */

#include <gtest/gtest.h>

#include <scene/Octree.hpp>
#include <scene/PolyMesh.hpp>

using namespace kocmoc::scene;

using glm::vec3;
using std::vector;


TEST(DummyTest, TestOne)
{
	Octree ot = Octree(vec3(0), 1);
	PolyMesh mesh(1, 1, 1);

	ot.insert(&mesh);
	vector<PolyMesh* > result = ot.getContent();

	ASSERT_EQ(result[0], &mesh);
}