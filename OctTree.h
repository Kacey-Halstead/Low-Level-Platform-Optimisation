#pragma once
#include "Vec3.h"
#include <vector>
#include <array>
#include <thread>
#include "ColliderObject.h"
#include <mutex>

namespace OctreeManager
{
	int GetCounter();
};

class OctTree
{
public:
	OctTree(Vec3 center, float halfSize, int maxRows, bool dynamicExpansion);
	~OctTree();

	void InsertObject(ColliderObject* obj);
	void ClearObjects();
	void ResolveCollisions();

	ColliderObject* start = nullptr;
	OctTree* parent = nullptr;
	std::array<OctTree*, 8> children;
	std::vector<std::thread> threads;
	std::thread thisThread;

private:
	void CreateChildren();
	int GetIndex(ColliderObject* obj);
	int GetNumObjects();
	void ResolveCollisionLock(OctTree* other);

	std::mutex octMutex;
	int objectCounter = 0;
	int numRows = 0;
	bool dynExp = false;
	Vec3 octcenter;
	float octHalfSize;
};
                                                                                                                                                                                                 