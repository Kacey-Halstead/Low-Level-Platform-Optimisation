#include "OctTree.h"
#include <iostream>
#include <cmath>

namespace OctTreeTracker
{
	int counter = 0;

	int ReturnCounter()
	{
		return counter;
	}

	void AddCounter()
	{
		counter++;
	}

};

OctTree::OctTree(Vec3 center, float halfSize, int numDivisions)
{
	octcenter = center;
	octHalfSize = halfSize;

	if (numDivisions > 1)
	{
		CreateChildren(center, halfSize / 2, numDivisions - 1);
	}
	OctTreeTracker::AddCounter();
	CalculateBounds();
}

OctTree::~OctTree()
{
	if (children[0] != nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			delete children[i];
		}
	}
}

bool OctTree::isInBounds(Vec3 pos)
{
	if (!(bounds[0] < pos.x < bounds[1]))
	{
		return false;
	}

	if (!(bounds[2] < pos.z < bounds[3]))
	{
		return false;
	}

	if (!(bounds[4] < pos.y < bounds[5]))
	{
		return false;
	}

	return true;
}

void OctTree::InsertObject(OctTree* treeNode, ColliderObject* obj)
{
	if (children[0] == nullptr) return;

	if (isLeaf)
	{
		objects.emplace_back(obj);
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			InsertObject(treeNode->children[i], obj);
		}
	}
}

void OctTree::ClearObjects()
{
	objects.clear();

	if (children[0] != nullptr)
	{
		for (OctTree* child : children)
		{
			child->ClearObjects();
		}
	}
}

std::vector<OctTree*> OctTree::GetLeafNodes(OctTree* oct)
{
	std::vector<OctTree*> leafNodes;

	if (isLeaf)
	{
		leafNodes.emplace_back(this);
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			GetLeafNodes(oct->children[i]);
		}
	}

	return leafNodes;
}

void OctTree::CreateChildren(Vec3 center, float halfSize, int numDivisions)
{
	children[0] = new OctTree(center + Vec3(-halfSize, -halfSize, halfSize), halfSize, numDivisions); //back left (top)
	children[1] = new OctTree(center + Vec3(halfSize, -halfSize, halfSize), halfSize, numDivisions); //back right (top)
	children[2] = new OctTree(center + Vec3(-halfSize, halfSize, halfSize), halfSize, numDivisions); //front left (top)
	children[3] = new OctTree(center + Vec3(halfSize, halfSize, halfSize), halfSize, numDivisions); //front right (top)
	children[4] = new OctTree(center + Vec3(-halfSize, -halfSize, -halfSize), halfSize, numDivisions); //back left (bottom)
	children[5] = new OctTree(center + Vec3(halfSize, -halfSize, -halfSize), halfSize, numDivisions); //back right (bottom)
	children[6] = new OctTree(center + Vec3(-halfSize, halfSize, -halfSize), halfSize, numDivisions); //front left (bottom)
	children[7] = new OctTree(center + Vec3(halfSize, halfSize, -halfSize), halfSize, numDivisions); //front right (bottom)

	for (int i = 0; i < 8; i++)
	{
		children[i]->parent = this;

		if (numDivisions == 1)
		{
			children[i]->isLeaf = true;
		}
	}
}

void OctTree::CalculateBounds()
{
	bounds[0] = octcenter.x - octHalfSize; //left side
	bounds[1] = octcenter.x + octHalfSize; //right side
	bounds[2] = octcenter.z - octHalfSize; //back
	bounds[3] = octcenter.z + octHalfSize; //front
	bounds[4] = octcenter.y - octHalfSize; //bottom
	bounds[5] = octcenter.y + octHalfSize; //top
}
