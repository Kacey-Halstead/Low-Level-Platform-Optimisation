#include "Tracker.h"

void Tracker::AddBytesAllocated(int numOfBytes)
{
	trackedAmount += numOfBytes;
}

void Tracker::RemoveBytesAllocated(int numOfBytes)
{
	trackedAmount -= numOfBytes;
}