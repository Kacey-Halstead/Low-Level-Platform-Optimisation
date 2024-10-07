#pragma once
#include<vector>

static class StaticClass
{
public:

	static std::vector<int> intVec[6];

	static void AddToVec(std::vector<int> toAdd);


};

