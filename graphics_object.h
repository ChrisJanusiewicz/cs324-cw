#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

//Class will contain graphics objects without textures
#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"
#include "game_component.h"

class graphics_object : public game_component {
public:

  graphics_object (std::vector<point3f> *vertices,
    std::vector<int> *vertex_indices,
    int color) {

      this->vertices = *vertices;
      this->vertex_indices = *vertex_indices;
      this->color = color;

  }
  virtual void display() {


    glBegin(GL_TRIANGLES);
      int n = 0;
      for (int i : vertex_indices) {
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
        /*std::cout << "vertex: " << n++
        << "\tindex: " << i
        << "\tx: " << vertices[i].x
        << "\ty: " << vertices[i].y
        << "\tz: " << vertices[i].z << std::endl;*/
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
