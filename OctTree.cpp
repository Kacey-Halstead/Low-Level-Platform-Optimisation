#include "OctTree.h"

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
}

OctTree::~OctTree()
{
	delete[] & children;
}

void OctTree::CreateChildren(Vec3 center, float halfSize, int numDivisions)
{
	children[0] = new OctTree(center + Vec3(-halfSize, -halfSize, halfSize), halfSize, numDivisions);
	children[1] = new OctTree(center + Vec3(halfSize, -halfSize, halfSize), halfSize, numDivisions);
	children[2] = new OctTree(center + Vec3(-halfSize, halfSize, halfSize), halfSize, numDivisions);
	children[3] = new OctTree(center + Vec3(halfSize, halfSize, halfSize), halfSize, numDivisions);
	children[4] = new OctTree(center + Vec3(-halfSize, -halfSize, -halfSize), halfSize, numDivisions);
	children[5] = new OctTree(center + Vec3(halfSize, -halfSize, -halfSize), halfSize, numDivisions);
	children[6] = new OctTree(center + Vec3(-halfSize, halfSize, -halfSize), halfSize, numDivisions);
	children[7] = new OctTree(center + Vec3(halfSize, halfSize, -halfSize), halfSize, numDivisions);

	for (int i = 0; i < 8; i++)
	{
		children[i]->parent = this;
	}
}

void OctTree::CalculateBounds()
{
	//center +/- half size for upper/lower bounds
}
