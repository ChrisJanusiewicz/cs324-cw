#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

//Class will contain graphics objects without textures
#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"

class graphics_object {
public:

  /*g_object () {
    //empty constructor?
  }*/
  graphics_object (std::vector<point3f> *vertices,
    std::vector<int> *vertex_indices,
    int color) {
      this->vertices = *vertices;
      this->vertex_indices = *vertex_indices;
      this->color = color;

  }
  void display() {

    

    glBegin(GL_TRIANGLES);
      for (int i : vertex_indices) {
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
      }
    glEnd();

  }

private:
  //Vertex list
  //Vertex indices corresponding to triangles making up the model:
  //First triangle - (vertices[0], vertices[1], vertices[2]), etc
  std::vector<point3f> vertices;
  std::vector<int> vertex_indices;
  //Color, later switch to material
  unsigned int color;
};

#endif
