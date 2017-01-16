#ifndef POINT3F_H
#define POINT3F_H

#include <cstdio>

struct point3f {
    float x;
    float y;
    float z;

    point3f() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    point3f(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    point3f operator-(const point3f& b) {
        return *new point3f(this->x - b.x, this->y - b.y, this->z - b.z);
    }
    point3f operator+(const point3f& b) {
        return *new point3f(this->x + b.x, this->y + b.y, this->z + b.z);
    }
    point3f operator*(const float f) {
        return *new point3f(this->x * f, this->y * f, this->z * f);
    }

    char* to_string(char *ret) {
        int n = sprintf(ret, "{%f, %f, %f}", x, y, z);
        return ret;
    }

    std::ostream& operator<<(std::ostream &stream) {
        return stream << "{" << x << ", " << y << ", " << z << "}";
    }

};

void normalise (point3f *p) {
    float magnitude = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
    p->x = p->x / magnitude;
    p->y = p->y / magnitude;
    p->z = p->z / magnitude;
}


#endif
