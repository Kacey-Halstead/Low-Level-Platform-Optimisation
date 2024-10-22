#pragma once
#include "Vec3.h"
#include <vector>
#include <array>
#include "ColliderObject.h"

namespace OctTreeTracker
{
	int ReturnCounter();
	void AddCounter();
};

class OctTree
{
public:
	OctTree(Vec3 center, float halfSize, int numDivisions);
	~OctTree();

	std::array<int, 6> bounds;

private:

	void CreateChildren(Vec3 center, float halfSize, int numDivisions);
	void CalculateBounds();

	Vec3 octcenter;
	float octHalfSize;
	OctTree* parent = nullptr;
	std::array<OctTree*, 8> children;
	std::vector<ColliderObject*> objects;
};

