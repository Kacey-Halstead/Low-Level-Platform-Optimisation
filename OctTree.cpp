#include "OctTree.h"
#include <iostream>
#include <cmath>
#include "ThreadManager.h"
#include <atomic>

#define MAX_OBJECTS 50

namespace OctreeManager
{
	int octTreeCount = 0;

	int GetCounter()
	{
		return octTreeCount;
	}
};

OctTree::OctTree(Vec3 center, float halfSize, int maxRows)
{
	OctreeManager::octTreeCount++;
	regionCentre = center;
	octHalfSize = halfSize;
	octreeDepth = maxRows;

	if (octreeDepth > 1)
	{
		CreateChildren();
	}
	else
	{
		for (int i = 0; i < childrenArr.size(); i++)
		{
			childrenArr[i] = nullptr;
		}
	}

}

OctTree::OctTree(Vec3 center, float halfSize, bool dynamicExpansion)
{
	OctreeManager::octTreeCount++;
	regionCentre = center;
	octHalfSize = halfSize;
	enableDynamicOctree = dynamicExpansion;
}

OctTree::~OctTree()
{
	for (int i = 0; i < childrenArr.size(); i++)
	{
		delete childrenArr[i];
		childrenArr[i] = nullptr;
	}

	parent = nullptr;
	objLinkedList = nullptr;
}

void OctTree::InsertObject(ColliderObject* obj)
{
	int index = GetIndex(obj); //gets index of child its in

	if (enableDynamicOctree) //if dynamic expansion
	{
		if (childrenArr[0] == nullptr) //is leaf
		{
			if (objsInRegion < MAX_OBJECTS)
			{
				if (index != 8)
				{
					objsInRegion++;
				}

				obj->next = objLinkedList;
				objLinkedList = obj;
				return;
			}
			else //if exceeds maxobj count, create children and insert
			{
				CreateChildren();
				ColliderObject* object = objLinkedList;
				objLinkedList = nullptr;
				while (object != nullptr)
				{
					InsertObject(object);
					object = object->next;
				}
			}
		}

		if (index == 8)
		{
			obj->next = objLinkedList;
			objLinkedList = obj;
		}
		else
		{ 
			childrenArr[index]->InsertObject(obj);
		}
	}
	else
	{
		if (index == 8 || childrenArr[index] == nullptr) //if straddling or child does not exist
		{
			obj->next = objLinkedList;
			objLinkedList = obj;
		}
		else //is child exists and obj is valid
		{
			childrenArr[index]->InsertObject(obj);
		}
	}
}

void OctTree::ClearObjects()
{
	objLinkedList = nullptr;

	if (childrenArr[0] == nullptr) return;

	for (int i = 0; i < 8; i++)
	{
		childrenArr[i]->ClearObjects();
	}

	objsInRegion = 0;
}

void OctTree::ResolveCollisionLock(OctTree* other)
{
	std::unique_lock<std::mutex> lock(other->octMutex);

	ColliderObject* curr = other->objLinkedList; //start of ancestor stack
	while (curr != nullptr)
	{
		//check against others in ancestor stack
		ColliderObject* nextObj = objLinkedList;
		while (nextObj != nullptr)
		{
			if (nextObj != curr)
			{
				curr->TestCollision(nextObj);
			}

			nextObj = nextObj->next;
		}
		curr = curr->next;
	}
}

void OctTree::ResolveCollisions()
{
	const int MAXDEPTH = 100;

	static OctTree* ancestorStack[MAXDEPTH];
	static int depth = 0;

	ancestorStack[depth++] = this;

	for (int i = 0; i < depth; i++) //for everything in ancestor stack
	{
		OctTree* other = ancestorStack[i];
		ManageThreads::Enqueue([this, other] {
			ResolveCollisionLock(other);
		});
	}

	if (childrenArr[0] != nullptr) //if has child, recurse 
	{
		for (int i = 0; i < 8; i++)
		{
			childrenArr[i]->ResolveCollisions();
		}
	}

	depth--;
}

void OctTree::CreateChildren()
{
	float childHalfSize = octHalfSize / 2;

	for (int i = 0; i < 8; i++) //creates all 8 children
	{
		Vec3 offset = Vec3(-childHalfSize, -childHalfSize, -childHalfSize);
		if (i % 2 != 0) // if even, its + halfsize, else - halfsize
		{
			offset.x = childHalfSize;
		}
		if (i == 2 || i == 3 || i == 6 || i == 7) //if these values, + y
		{
			offset.y = childHalfSize;
		}
		if (i > 3) //if above 3, + halfsize on z
		{
			offset.z = childHalfSize;
		}

		if (enableDynamicOctree) //if dynamic expansion
		{
			childrenArr[i] = new OctTree(regionCentre + offset, childHalfSize, enableDynamicOctree);
		}
		else
		{
			childrenArr[i] = new OctTree(regionCentre + offset, childHalfSize, octreeDepth - 1); //creates correct offset for different children
		}
		childrenArr[i]->parent = this;
	}

	/* positions of regions
	children[0] = back left (bottom)
	children[1] = back right (bottom)
	children[2] = front left (bottom)
	children[3] = front right (bottom)
	children[4] = back left (top)
	children[5] = back right (top)
	children[6] = front left (top)
	children[7] = front right (top)
	*/
}

int OctTree::GetIndex(ColliderObject* obj)
{
	int index = 0; //adds 1, 2, or 4 depending if  > or < than centre

	if (CheckStraddling(obj->position.x, obj->size.x, regionCentre.x) || CheckStraddling(obj->position.z, obj->size.z, regionCentre.z)) return 8;

	if (obj->position.x > regionCentre.x)
	{
		index += 1;
	}
	if (obj->position.y > regionCentre.y)
	{
		index += 2;
	}
	if (obj->position.z > regionCentre.z)
	{
		index += 4;
	}

	return index;
}

bool OctTree::CheckStraddling(float pos, float size, float region)
{
	if (pos + size > region && pos - size < region) //check for straddling
	{
		return true;
	}

	return false;
}