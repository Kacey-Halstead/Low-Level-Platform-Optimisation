#pragma once
#include "Vec3.h"
#include <vector>
#include <array>
#include "ColliderObject.h"

class OctTree
{
public:
	OctTree(Vec3 center, float halfSize, int maxRows);
	~OctTree();

	void InsertObject(ColliderObject* obj);
	void ClearObjects();
	void ResolveCollisions();

	ColliderObject* start = nullptr;
	OctTree* parent = nullptr;
	std::array<OctTree*, 8> children;

private:

	void CreateChildren(int maxRows);
	int GetIndex(ColliderObject* obj);

	Vec3 octcenter;
	float octHalfSize;
};
                                                                                                                                                                                                 