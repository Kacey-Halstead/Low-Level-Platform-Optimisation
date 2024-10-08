#pragma once

enum Types
{
	DEFAULT,
	CUBE,
	SPHERE
};

namespace Tracker
{
	int trackedAmount;

	void AddBytesAllocated(int numOfBytes)
	{
		trackedAmount += numOfBytes;
	}

	void RemoveBytesAllocated(int numOfBytes)
	{
		trackedAmount -= numOfBytes;
	}
};

namespace CubeTracker 
{
	int trackedAmount;

	void AddBytesAllocated(int numOfBytes)
	{
		trackedAmount += numOfBytes;
	}

	void RemoveBytesAllocated(int numOfBytes)
	{
		trackedAmount -= numOfBytes;
	}
};

namespace SphereTracker
{
	int trackedAmount;

	void AddBytesAllocated(int numOfBytes)
	{
		trackedAmount += numOfBytes;
	}

	void RemoveBytesAllocated(int numOfBytes)
	{
		trackedAmount -= numOfBytes;
	}
};


