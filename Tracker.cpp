#include "Tracker.h"

namespace Tracker
{
	int DefaultTrackedAmount = 0;
	int CubeTrackedAmount = 0;
	int SphereTrackedAmount = 0;

	int GetTrackedAmount(Types type)
	{
		switch (type)
		{
		case DEFAULT:
			return DefaultTrackedAmount;
			break;
		case CUBE:
			return CubeTrackedAmount;
			break;
		case SPHERE:
			return SphereTrackedAmount;
			break;
		}

		return 0;
	}

	std::string GetName(Types type)
	{
		switch (type)
		{
		case DEFAULT:
			return "Default";
			break;
		case CUBE:
			return "Cube";
			break;
		case SPHERE:
			return "Sphere";
			break;
		}

		return std::string();
	}

	void AddBytesAllocated(int numOfBytes, Types type)
	{
		switch (type)
		{
		case DEFAULT:
			DefaultTrackedAmount += numOfBytes;
			break;
		case CUBE:
			CubeTrackedAmount += numOfBytes;
			break;
		case SPHERE:
			SphereTrackedAmount += numOfBytes;
			break;
		}
	}

	void RemoveBytesAllocated(int numOfBytes, Types type)
	{
		switch (type)
		{
		case DEFAULT:
			DefaultTrackedAmount -= numOfBytes;
			break;
		case CUBE:
			CubeTrackedAmount -= numOfBytes;
			break;
		case SPHERE:
			SphereTrackedAmount -= numOfBytes;
			break;
		}
	}
};