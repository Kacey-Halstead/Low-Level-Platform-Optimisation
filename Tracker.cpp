#include "Tracker.h"

namespace Tracker
{
	int trackedAmount = 0;

	int GetTrackedAmount()
	{
		return trackedAmount;
	}

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
	int trackedAmount = 0;

	int GetTrackedAmount()
	{
		return trackedAmount;
	}

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
	int trackedAmount = 0;

	int GetTrackedAmount()
	{
		return trackedAmount;
	}

	void AddBytesAllocated(int numOfBytes)
	{
		trackedAmount += numOfBytes;
	}

	void RemoveBytesAllocated(int numOfBytes)
	{
		trackedAmount -= numOfBytes;
	}
};