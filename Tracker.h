#pragma once

enum Types
{
	DEFAULT,
	CUBE,
	SPHERE
};

namespace Tracker
{
	int GetTrackedAmount();

	void AddBytesAllocated(int numOfBytes);

	void RemoveBytesAllocated(int numOfBytes);
};

namespace CubeTracker 
{

	int GetTrackedAmount();

	void AddBytesAllocated(int numOfBytes);

	void RemoveBytesAllocated(int numOfBytes);
};

namespace SphereTracker
{
	int GetTrackedAmount();

	void AddBytesAllocated(int numOfBytes);

	void RemoveBytesAllocated(int numOfBytes);
};


