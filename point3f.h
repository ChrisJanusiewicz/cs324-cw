#ifndef POINT3F_H
#define POINT3F_H

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

/*  std::ostream& operator<<(std::ostream &stream, const point3f &p) {
    return stream << "{" << x << ", " << y << ", " << z << "}"
  } */

};

#endif
