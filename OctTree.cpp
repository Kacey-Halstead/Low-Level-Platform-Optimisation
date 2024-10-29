#include "OctTree.h"
#include <iostream>
#include <cmath>

OctTree::OctTree(Vec3 center, float halfSize, int maxRows)
{
	octcenter = center;
	octHalfSize = halfSize;
	if (maxRows > 1)
	{
		CreateChildren(maxRows-1);
	}
}

OctTree::~OctTree()
{

}

void OctTree::InsertObject(ColliderObject* obj)
{
	int index = GetIndex(obj); //gets index of child its in

	if (index == 8 || children[index] == nullptr)
	{
		obj->next = start;
		start = obj;
	}
	else
	{
		children[index]->InsertObject(obj);
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

void OctTree::ResolveCollisions()
{
	const int MAXDEPTH = 5;
	static OctTree* ancestorStack[MAXDEPTH];
	static int depth = 0;

	ancestorStack[depth++] = this;

	//must check against others in linked list and ancestor stack

	for (int i = 0; i < depth; i++) //for everything in ancestor stack
	{
		ColliderObject* curr = ancestorStack[i]->start; //start of ancestor stack

		while (curr != nullptr)
		{
			//check against others in ancestor stack
			ColliderObject* nextObj;
			nextObj = curr->next;
			while (nextObj != nullptr)
			{
				curr->TestCollision(nextObj);
				nextObj = nextObj->next;
			}

			//checks against normal linked list
			nextObj = start;
			while (nextObj != nullptr)
			{
				curr->TestCollision(nextObj);
				nextObj = nextObj->next;
			}
			curr = curr->next;
		}
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

void OctTree::CreateChildren(int maxRows)
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
		children[i] = new OctTree(octcenter + offset, halfSize, maxRows); //creates correct offset for different children
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