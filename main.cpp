#include <stdlib.h>
#include <GL/glut.h>
#include <list>
#include <iostream>

#include <cstdlib>
#include <ctime>
#include <chrono>

#include "globals.h"
#include "Vec3.h"
#include "ColliderObject.h"
#include "Box.h"
#include "Sphere.h"
#include "MemoryAllocation.h"
#include "MemoryPool.h"
#include "StaticClass.h"
#include "Tracker.h"
#include "Timer.h"


using namespace std::chrono;

// this is the number of falling physical items. 
#define NUMBER_OF_BOXES 50
#define NUMBER_OF_SPHERES 50

// these is where the camera is, where it is looking and the bounds of the continaing box. You shouldn't need to alter these

#define LOOKAT_X 10
#define LOOKAT_Y 10
#define LOOKAT_Z 50

#define LOOKDIR_X 10
#define LOOKDIR_Y 0
#define LOOKDIR_Z 0


std::list<ColliderObject*> colliders;



float GenerateRandom(float toDivide)
{
    return (static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / toDivide)));
}

inline ColliderObject* CreateObj(bool isBox)
{
    ColliderObject* object;
    if (isBox)
    {
        object = new Box;
        object->typeName = "Box";
    }
    else
    {
        object = new Sphere;
        object->typeName = "Sphere";
    }

    // Assign random x, y, and z positions within specified ranges
    object->position = { GenerateRandom(20), 10.0f + GenerateRandom(1), GenerateRandom(20) };

    object->size = { 1.0f, 1.0f, 1.0f };

    // Assign random x-velocity between -1.0f and 1.0f
    object->velocity = { -1 + GenerateRandom(2), 0.0f, 0.0f };

    // Assign a random colour to the object
    object->colour = { GenerateRandom(1), GenerateRandom(1), GenerateRandom(1) };
    return object;
}


void initScene(const int &boxCount, const int &sphereCount) { //const refs because values do not need to be changed 
    Timer::StartTimer();

    for (int i = boxCount; i--;) { //faster to check if = 0, quicker than i < boxCount
        colliders.emplace_back(CreateObj(true));
    }

    for (int i = sphereCount; i--;) {
        colliders.emplace_back(CreateObj(false));
    }

    Timer::EndTimer();
}

// a ray which is used to tap (by default, remove) a box - see the 'mouse' function for how this is used.
bool rayBoxIntersection(const Vec3& rayOrigin, const Vec3& rayDirection, const ColliderObject* box) {
    float tMin = (box->position.x - box->size.x / 2.0f - rayOrigin.x) / rayDirection.x;
    float tMax = (box->position.x + box->size.x / 2.0f - rayOrigin.x) / rayDirection.x;

    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (box->position.y - box->size.y / 2.0f - rayOrigin.y) / rayDirection.y;
    float tyMax = (box->position.y + box->size.y / 2.0f - rayOrigin.y) / rayDirection.y;

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax))
        return false;

    if (tyMin > tMin)
        tMin = tyMin;

    if (tyMax < tMax)
        tMax = tyMax;

    float tzMin = (box->position.z - box->size.z / 2.0f - rayOrigin.z) / rayDirection.z;
    float tzMax = (box->position.z + box->size.z / 2.0f - rayOrigin.z) / rayDirection.z;

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return false;

    return true;
}

// used in the 'mouse' tap function to convert a screen point to a point in the world
Vec3 screenToWorld(int x, int y) {
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    return Vec3((float)posX, (float)posY, (float)posZ);
}




// update the physics: gravity, collision test, collision resolution
void updatePhysics(const float deltaTime) {
    
    // todo for the assessment - use a thread for each sub region
    // for example, assuming we have two regions:
    // from 'colliders' create two separate lists
    // empty each list (from previous frame) and work out which collidable object is in which region, 
    //  and add the pointer to that region's list.
    // Then, run two threads with the code below (changing 'colliders' to be the region's list)

    for (ColliderObject* box : colliders) { 
        
        box->update(&colliders, deltaTime);
        
    }
}

// draw the sides of the containing area
void drawQuad(const Vec3& v1, const Vec3& v2, const Vec3& v3, const Vec3& v4) {
    
    glBegin(GL_QUADS);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v4.x, v4.y, v4.z);
    glEnd();
}



// draw the entire scene
void drawScene() {

    // Draw the side wall
    GLfloat diffuseMaterial[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);

    // Draw the left side wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vec3 leftSideWallV1(minX, 0.0f, maxZ);
    Vec3 leftSideWallV2(minX, 50.0f, maxZ);
    Vec3 leftSideWallV3(minX, 50.0f, minZ);
    Vec3 leftSideWallV4(minX, 0.0f, minZ);
    drawQuad(leftSideWallV1, leftSideWallV2, leftSideWallV3, leftSideWallV4);

    // Draw the right side wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vec3 rightSideWallV1(maxX, 0.0f, maxZ);
    Vec3 rightSideWallV2(maxX, 50.0f, maxZ);
    Vec3 rightSideWallV3(maxX, 50.0f, minZ);
    Vec3 rightSideWallV4(maxX, 0.0f, minZ);
    drawQuad(rightSideWallV1, rightSideWallV2, rightSideWallV3, rightSideWallV4);


    // Draw the back wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vec3 backWallV1(minX, 0.0f, minZ);
    Vec3 backWallV2(minX, 50.0f, minZ);
    Vec3 backWallV3(maxX, 50.0f, minZ);
    Vec3 backWallV4(maxX, 0.0f, minZ);
    drawQuad(backWallV1, backWallV2, backWallV3, backWallV4);

    for (ColliderObject* box : colliders) {
        box->draw();
    }
}

// called by GLUT - displays the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(LOOKAT_X, LOOKAT_Y, LOOKAT_Z, LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z, 0, 1, 0);

    drawScene();

    glutSwapBuffers();
}

// called by GLUT when the cpu is idle - has a timer function you can use for FPS, and updates the physics
// see https://www.opengl.org/resources/libraries/glut/spec3/node63.html#:~:text=glutIdleFunc
// NOTE this may be capped at 60 fps as we are using glutPostRedisplay(). If you want it to go higher than this, maybe a thread will help here. 
void idle() {
    static auto last = steady_clock::now();
    auto old = last;
    last = steady_clock::now();
    const duration<float> frameTime = last - old;
    float deltaTime = frameTime.count();

    updatePhysics(deltaTime);

    // tell glut to draw - note this will cap this function at 60 fps
    glutPostRedisplay();
}

// called the mouse button is tapped
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Get the camera position and direction
        Vec3 cameraPosition(LOOKAT_X, LOOKAT_Y, LOOKAT_Z); // Replace with your actual camera position
        Vec3 cameraDirection(LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z); // Replace with your actual camera direction

        // Get the world coordinates of the clicked point
        Vec3 clickedWorldPos = screenToWorld(x, y);

        // Calculate the ray direction from the camera position to the clicked point
        Vec3 rayDirection = clickedWorldPos - cameraPosition;
        rayDirection.normalise();

        // Perform a ray-box intersection test and remove the clicked box
        bool clickedBoxOK = false;
        float minIntersectionDistance = std::numeric_limits<float>::max();

        ColliderObject* clickedBox = nullptr;
        for (ColliderObject* box : colliders) {
            if (rayBoxIntersection(cameraPosition, rayDirection, box)) {
                // Calculate the distance between the camera and the intersected box
                Vec3 diff = box->position - cameraPosition;
                float distance = diff.length();

                // Update the clicked box index if this box is closer to the camera
                if (distance < minIntersectionDistance) {
                    clickedBoxOK = true;
                    minIntersectionDistance = distance;
                    clickedBox = box;
                }
            }
        }

        // Remove the clicked box if any
        if (clickedBoxOK != false) {
            colliders.remove(clickedBox);
        }
    }
}

// called when the keyboard is used
void keyboard(unsigned char key, int x, int y) {
    const float impulseMagnitude = 20.0f; // Upward impulse magnitude  

    switch (key)
    {
    case ' ': //space bar
        for (ColliderObject* box : colliders) {
            box->velocity.y += impulseMagnitude;
        }
        break;

    case '1':
        std::cout << "Memory used" << std::endl;
        break;

    case 'w':
        MemoryAlloc::WalkTheHeap();
        break;

    case 'f':
    {
        char* Footername = new char[10];
        const char* toCopy = "aaaaaaaaaaaaaa";
        memcpy(Footername, toCopy, 15);
        std::cout << "\nEnforced footer buffer overflow" << std::endl;
    }
        break;

    case 'h':
    {
        char* Headername = new char[10];
        const char* toCopy = "aaaaaaaaaaaaaa";
        memcpy(Headername-5, toCopy, 15);
        std::cout << "\nEnforced header buffer overflow" << std::endl;
    }
    break;

    case 'r': //remove box
    {
        if (colliders.size() <= 0 || colliders.front()->typeName != "Box") break;
        Box* box = (Box*)colliders.front();
        delete box;
        colliders.pop_front();
        std::cout << "\nRemoved box" << std::endl;
    }
        break;

    case 'a': //add box
    {
        colliders.emplace_front(CreateObj(true));
        std::cout << "\nAdded box" << std::endl;
    }
        break;

    case 'A': //add sphere
    {
        colliders.emplace_back(CreateObj(false));
        std::cout << "\nAdded sphere" << std::endl;
    }
        break;

    case 'R': //remove sphere
    {
        if (colliders.size() <= 0 || colliders.back()->typeName != "Sphere") break;
        Sphere* sphere = (Sphere*)colliders.back();
        delete sphere;
        colliders.pop_back();
        std::cout << "\nRemoved sphere" << std::endl;
    }
        break;

    case 'm':
    {
        std::cout << "\nDefault tracker: " << Tracker::GetTrackedAmount() << std::endl;
        std::cout << "Sphere tracker: " << SphereTracker::GetTrackedAmount() << std::endl;
        std::cout << "Cube tracker: " << CubeTracker::GetTrackedAmount() << std::endl;

        MemoryPool* bMemPool = MemoryAlloc::GetPool(0);
        MemoryPool* mMemPool = MemoryAlloc::GetPool(1);
        MemoryPool* sMemPool = MemoryAlloc::GetPool(2);
        std::cout << "Total memory used in memory pool 1: " << bMemPool->memUsed << "/" << bMemPool->poolSize << std::endl;
        std::cout << "Total memory used in memory pool 2: " << mMemPool->memUsed << "/" << mMemPool->poolSize << std::endl;
        std::cout << "Total memory used in memory pool 3: " << sMemPool->memUsed << "/" << sMemPool->poolSize << std::endl;
    }
        break;
    }
}

// the main function. 
int main(int argc, char** argv) {

    srand(static_cast<unsigned>(time(0))); // Seed random number generator
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Simple Physics Simulation");

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    initScene(NUMBER_OF_BOXES, NUMBER_OF_SPHERES);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    // it will stick here until the program ends. 
    glutMainLoop();
    return 0;
}
