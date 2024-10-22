#pragma once
#include <string>

enum Types
{
	DEFAULT,
	CUBE,
	SPHERE
};

namespace Tracker
{
	int GetTrackedAmount(Types type);

	std::string GetName(Types type);

	void AddBytesAllocated(int numOfBytes, Types type);

	void RemoveBytesAllocated(int numOfBytes, Types type);
};