#include "Tracker.h"

namespace Tracker
{
	int DefaulttrackedAmount = 0;
	int CubetrackedAmount = 0;
	int SpheretrackedAmount = 0;

	int GetTrackedAmount(Types type)
	{
		switch (type)
		{
		case DEFAULT:
			return DefaulttrackedAmount;
			break;
		case CUBE:
			return CubetrackedAmount;
			break;
		case SPHERE:
			return SpheretrackedAmount;
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
			DefaulttrackedAmount += numOfBytes;
			break;
		case CUBE:
			CubetrackedAmount += numOfBytes;
			break;
		case SPHERE:
			SpheretrackedAmount += numOfBytes;
			break;
		}
	}

	void RemoveBytesAllocated(int numOfBytes, Types type)
	{
		switch (type)
		{
		case DEFAULT:
			DefaulttrackedAmount -= numOfBytes;
			break;
		case CUBE:
			CubetrackedAmount -= numOfBytes;
			break;
		case SPHERE:
			SpheretrackedAmount -= numOfBytes;
			break;
		}
	}
};