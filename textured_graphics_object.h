#ifndef TEXTURED_GRAPHICS_OBJECT_H
#define TEXTURED_GRAPHICS_OBJECT_H

//Class will contain graphics objects without textures
#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"
#include "point2f.h"
#include "graphics_object.h"

class textured_graphics_object : public graphics_object {
public:

  textured_graphics_object (std::vector<point3f> *vertices,
    std::vector<int> *vertex_indices,
    std::vector<point2f> *tex_coords,
    std::vector<int> *tex_indices,
    unsigned int *tex_handle) : graphics_object(vertices, vertex_indices, 0xFFFF00FF) {


    this->vertices = *vertices;
    this->vertex_indices = *vertex_indices;
    this->tex_coords = *tex_coords;
    this->tex_indices = *tex_indices;
    this->tex_handle = *tex_handle;

    std::cout << "wtf" << std::endl;
    calc_normals();

  }
  void display() override {
    //std::cout << "Attempting to display textured graphics_object" << std::endl;


  	// enable texturing and bind
  	glEnable(GL_TEXTURE_2D);
  	//glEnable(GL_LIGHTING);
  	glBindTexture(GL_TEXTURE_2D, tex_handle);


    glBegin(GL_TRIANGLES);
      int n = tex_indices.size();
      int m = 0;
      //std::cout << "Number of vertex indices: " << n << std::endl;
      for (int i = 0; i < n; i++) {
        point3f &normal = this->vertex_normals[i];
        point3f &vertex = this->vertices[vertex_indices[i]];
        point2f &tex = this->tex_coords[tex_indices[i]];


        glNormal3f(normal.x, normal.y, normal.z);
        glTexCoord2f(tex.x, tex.y);
        glVertex3f(vertex.x, vertex.y, vertex.z);
      }

    glEnd();


    //glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
      /*
    //DRAW VECTOR NORMALS
    glBegin(GL_LINES);
    for (int i = 0; i < n; i++) {
      point3f &normal = this->vertex_normals[i];
      point3f &vertex = this->vertices[vertex_indices[i]];

      glVertex3f(vertex.x, vertex.y, vertex.z);
      glVertex3f(vertex.x + normal.x, vertex.y + normal.y, vertex.z + normal.z);
    }*/


    glEnd();

  	glDisable(GL_TEXTURE_2D);

    //std::cout << "Finished displaying textured graphics_object" << std::endl;

  }

  void calc_normals() {
    std::cout << "starting to calculate normals" << std::endl;
    point3f *result = new point3f();
    int i = 0;
    while (i < vertex_indices.size()) {
      point3f v1 = vertices[vertex_indices[i]];
      point3f v2 = vertices[vertex_indices[i + 1]];
      point3f v3 = vertices[vertex_indices[i + 2]];
      point3f u = v2 - v1;
      point3f v = v3 - v1;
      point3f normal;
      cross_product(&u, &v, &normal);
      normalise(&normal);
      //vertex_normals.push_back(*new point3f(0.0f, 1.0f, 0.0f));
      vertex_normals.push_back(normal);
      vertex_normals.push_back(normal);
      vertex_normals.push_back(normal);
      i += 3;
    }
  }

private:
  //Vertex list
  //Vertex indices corresponding to triangles making up the model:
  //First triangle - (vertices[0], vertices[1], vertices[2]), etc
  std::vector<point3f> vertices;
  std::vector<int> vertex_indices;
  std::vector<point3f> vertex_normals;
  std::vector<point2f> tex_coords;
  std::vector<int> tex_indices;
  unsigned int tex_handle;
};

#endif
