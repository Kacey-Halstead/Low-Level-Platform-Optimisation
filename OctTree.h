#pragma once
#include <array>
#include "ColliderObject.h"
#include <mutex>

namespace OctreeManager
{
	int GetCounter();
}

class OctTree
{
public:
	OctTree(Vec3 center, float halfSize, int maxRows);
	OctTree(Vec3 center, float halfSize, bool dynamicExpansion);
	~OctTree();

	void InsertObject(ColliderObject* obj);
	void ClearObjects();
	void ResolveCollisions();

private:
	void CreateChildren();
	int GetIndex(ColliderObject* obj);
	inline bool CheckStraddling(float pos, float size, float region);
	void ResolveCollisionLock(OctTree* other);

	std::mutex octMutex;

	std::array<OctTree*, 8> childrenArr;
	Vec3 regionCentre{};

	int objsInRegion = 0;
	int octreeDepth = 0;
	int numThreadCount = 0;

	float octHalfSize = 0;

	bool enableDynamicOctree = false;

	ColliderObject* objLinkedList = nullptr;
	OctTree* parent = nullptr;
};
                                                                                                                                                                                                 