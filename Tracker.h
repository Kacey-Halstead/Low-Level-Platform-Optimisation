#pragma once

static class Tracker
{
public:
	int trackedAmount;

	void AddBytesAllocated(int numOfBytes);
	void RemoveBytesAllocated(int numOfBytes);
};

static class CubeTracker : public Tracker
{

};

static class SphereTracker : public Tracker
{

};


