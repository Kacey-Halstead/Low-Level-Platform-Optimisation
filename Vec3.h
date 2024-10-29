#pragma once

#include <cmath>

class Vec3 {
public:
    float x, y, z;

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3(float val) : x(val), y(val), z(val) {}

    // overload the minus operator
    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator/(const float& other) const {
        return Vec3(x / other, y / other, z / other);
    }

    bool operator>(const float& other) const {
        return (x > other, y > other, z > other);
    }

    // Normalize the vector
    void normalise() {
        float length = std::sqrt(x * x + y * y + z * z);
        if (length != 0) {
            x /= length;
            y /= length;
            z /= length;
        }
    }

    // get the length of a vector
    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
};

