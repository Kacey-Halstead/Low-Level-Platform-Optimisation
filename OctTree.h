#pragma once
#include "Vec3.h"
#include <vector>
#include <array>
#include "ColliderObject.h"

class OctTree
{
public:
	OctTree(Vec3 center, float halfSize, int numDivisions);
	~OctTree();

	bool isInBounds(Vec3 pos);
	void InsertObject(OctTree* treeNode, ColliderObject* obj);
	void ClearObjects();
	std::vector<OctTree*> GetLeafNodes(OctTree* oct);

	std::array<float, 6> bounds;
	std::vector<ColliderObject*> objects;
	OctTree* parent = nullptr;
	std::array<OctTree*, 8> children;

private:

	void CreateChildren(Vec3 center, float halfSize, int numDivisions);
	void CalculateBounds();
	bool isLeaf = false;
	Vec3 octcenter;
	float octHalfSize;
};

namespace OctTreeTracker
{
	int ReturnCounter();
	void AddCounter();
};
