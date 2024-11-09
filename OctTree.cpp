#include "OctTree.h"
#include <iostream>
#include <cmath>
#include "ThreadManager.h"

#define MAX_OBJECTS 50

namespace OctreeManager
{
	int octTreeCount = 0;

	int GetCounter()
	{
		return octTreeCount;
	}
};

OctTree::OctTree(Vec3 center, float halfSize, int maxRows, bool dynamicExpansion)
{
	ManageThreads::Init(10);
	OctreeManager::octTreeCount++;
	octcenter = center;
	octHalfSize = halfSize;
	dynExp = dynamicExpansion;
	numRows = maxRows;

	if (!dynExp) //down to specific depth
	{
		if (numRows > 1)
		{
			CreateChildren();
		}
	}
}

OctTree::~OctTree()
{
	ManageThreads::Destroy();
}

void OctTree::InsertObject(ColliderObject* obj)
{
	int index = GetIndex(obj); //gets index of child its in

	if (dynExp) //if dynamic expansion
	{
		if (children[0] == nullptr) //is leaf
		{
			if (objectCounter < MAX_OBJECTS)
			{
				if (index != 8)
				{
					objectCounter++;
				}

				obj->next = start;
				start = obj;
				return;
			}
			else //if exceeds maxobj count, create children and insert
			{
				CreateChildren();
				ColliderObject* object = start;
				start = nullptr;
				while (object != nullptr)
				{
					InsertObject(object);
					object = object->next;
				}
			}
		}

		if (index == 8)
		{
			obj->next = start;
			start = obj;
		}
		else
		{ 
			children[index]->InsertObject(obj);
		}
	}
	else
	{
		if (index == 8 || children[index] == nullptr) //if straddling or child does not exist
		{
			obj->next = start;
			start = obj;
		}
		else //is child exists and obj is valid
		{
			children[index]->InsertObject(obj);
		}
	}
}

void OctTree::ClearObjects()
{
	start = nullptr;

	if (children[0] == nullptr) return;

	for (int i = 0; i < 8; i++)
	{
		children[i]->ClearObjects();
	}
}

void OctTree::ResolveCollisionLock(OctTree* other)
{
	other->octMutex.lock();
	ColliderObject* curr = other->start; //start of ancestor stack
	while (curr != nullptr)
	{
		//check against others in ancestor stack
		ColliderObject* nextObj = start;
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
	other->octMutex.unlock();
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

	if (children[0] != nullptr) //if has child, recurse 
	{
		for (int i = 0; i < 8; i++)
		{
			children[i]->ResolveCollisions();
		}
	}

	depth--;
}

void OctTree::CreateChildren()
{
	float halfSize = octHalfSize / 2;

	for (int i = 0; i < 8; i++) //creates all 8 children
	{
		Vec3 offset = Vec3(-halfSize, -halfSize, -halfSize);
		if (i % 2 != 0) // if even, its + halfsize, else - halfsize
		{
			offset.x = halfSize;
		}
		if (i == 2 || i == 3 || i == 6 || i == 7) //if these values, + y
		{
			offset.y = halfSize;
		}
		if (i > 3) //if above 3, + halfsize on z
		{
			offset.z = halfSize;
		}

		if (dynExp) //is dynamic expansion
		{
			children[i] = new OctTree(octcenter + offset, halfSize, numRows, dynExp);
		}
		else
		{
			if (numRows > 1)
			{
				children[i] = new OctTree(octcenter + offset, halfSize, numRows - 1, dynExp); //creates correct offset for different children
			}
		}
		children[i]->parent = this;
	}

	//children[0] = back left (bottom)
	//children[1] = back right (bottom)
	//children[2] = front left (bottom)
	//children[3] = front right (bottom)
	//children[4] = back left (top)
	//children[5] = back right (top)
	//children[6] = front left (top)
	//children[7] = front right (top)
}

int OctTree::GetIndex(ColliderObject* obj)
{
	int index = 0; //adds 1, 2, or 4 depending if  > or < than centre

	if (obj->position.x + obj->size.x > octcenter.x && obj->position.x - obj->size.x < octcenter.x) //check for straddling
	{
		return 8;
	}

	if (obj->position.z + obj->size.z > octcenter.z && obj->position.z - obj->size.z < octcenter.z) //check for straddling
	{
		return 8;
	}


	if (obj->position.x > octcenter.x)
	{
		index += 1;
	}
	if (obj->position.y > octcenter.y)
	{
		index += 2;
	}
	if (obj->position.z > octcenter.z)
	{
		index += 4;
	}



	return index;
}

int OctTree::GetNumObjects()
{
	int counter = 0;
	ColliderObject* obj = start;

	while (obj != nullptr)
	{
		counter++;
		obj = obj->next;
	}

	return counter;
}
