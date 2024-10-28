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

	if (children[index] != nullptr) //if has child, recurse 
	{
		children[index]->InsertObject(obj);
	}
	else //if leaf node, add object to linked list
	{
		obj->next = start;
		start = obj;
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
	ColliderObject* curr = start;

	while (curr != nullptr) //gets object in linked list
	{
		ColliderObject* nextObj = curr->next;
		while (nextObj != nullptr) //checks object against every other object in linked list
		{
			if (curr == nextObj) continue;
			curr->TestCollision(nextObj);
			nextObj = nextObj->next;
		}
		curr = curr->next;
	}

	if (children[0] != nullptr) //if has child, recurse 
	{
		for (int i = 0; i < 8; i++)
		{
			children[i]->ResolveCollisions();
		}
	}
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
