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

  char* to_string() {
    char str[50];
    int n = sprintf(str, "{%f, %f, %f}", x, y, z);
    return str;
  }

  std::ostream& operator<<(std::ostream &stream) {
    return stream << "{" << x << ", " << y << ", " << z << "}";
  }

};

#endif
