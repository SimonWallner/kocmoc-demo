/*
 * The basic scene class that holds all the objects.
 * This is not a scene graph, it is rather a set of relevant objects.
 */

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <list>
#include "PolyMesh.hpp"

class Scene
{
public:
	Scene();
	virtual ~Scene();

private:
	std::list<PolyMesh> meshes;
};

#endif /* SCENE_HPP_ */
