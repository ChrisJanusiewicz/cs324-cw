#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

//Class will contain graphics objects without textures
#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"
#include "game_component.h"
#include "cross_product.h"

class graphics_object : public game_component {
public:

  graphics_object (std::vector<point3f> *vertices,
    std::vector<int> *vertex_indices,
    int color) {

      this->vertices = *vertices;
      this->vertex_indices = *vertex_indices;
      this->color = color;

      calc_normals();

  }
  virtual void display() {


    glBegin(GL_TRIANGLES);
      int n = 0;
      for (int i : vertex_indices) {
        glNormal3f(vertex_normals[i].x, vertex_normals[i].y, vertex_normals[i].z);
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
      }
    glEnd();


  }
  void calc_normals() {
    std::cout << "starting to calculate normals" << std::endl;
    point3f *result = new point3f();
    int i = 0;
    while (i < vertex_indices.size()) {
      point3f v1 = vertices[vertex_indices[i]];
      point3f v2 = vertices[vertex_indices[i + 1]];
      point3f v3 = vertices[vertex_indices[i + 2]];
      vertex_normals.push_back(*new point3f(0.0f, 1.0f, 0.0f));
      vertex_normals.push_back(*new point3f(0.0f, 1.0f, 0.0f));
      vertex_normals.push_back(*new point3f(0.0f, 1.0f, 0.0f));
      i += 3;
    }

    std::cout << vertex_normals.size() << std::endl;
    std::cout << "Done calculating normals" << std::endl;
  }

private:
  //Vertex list
  std::vector<point3f> vertices;
  //Vertex indices corresponding to triangles making up the model:
  //First triangle - (vertices[0], vertices[1], vertices[2]), etc
  std::vector<int> vertex_indices;
  //vertex normal list
  std::vector<point3f> vertex_normals;
  //Color, later switch to material
  unsigned int color;
};

#endif
