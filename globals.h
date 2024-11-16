#pragma once
#define minX -10.0f
#define maxX 30.0f
#define minZ -30.0f
#define maxZ 30.0f

// these is where the camera is, where it is looking and the bounds of the continaing box. You shouldn't need to alter these
#define LOOKAT_X 10
#define LOOKAT_Y 10
#define LOOKAT_Z 50

#define LOOKDIR_X 10
#define LOOKDIR_Y 0
#define LOOKDIR_Z 0

// gravity - change it and see what happens (usually negative!)
constexpr static float gravity = -19.81f;

//vals inputted by user
struct InitValues 
{
    bool dynamicOctree = false;
    int octreeSize = 0;
    int cubeCount = 0;
    int sphereCount = 0;
    int numThreads = 0;
};

#include<array>
//keeps track of average FPS
struct FPSTimer
{
    std::array<float, 30> FPSTimes{};
    int curFrameCounter = 0;
};