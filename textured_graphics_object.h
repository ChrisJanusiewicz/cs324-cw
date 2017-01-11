#ifndef TEXTURED_GRAPHICS_OBJECT_H
#define TEXTURED_GRAPHICS_OBJECT_H

//Class will contain graphics objects without textures
#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"
#include "point2f.h"

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
      std::cout << vertices->size() << std::endl;
      std::cout << vertex_indices->size() << std::endl;
      std::cout << tex_coords->size() << std::endl;
      std::cout << tex_indices->size() << std::endl;
      std::cout << *tex_handle << std::endl;

  }
  void display() override {
    //std::cout << "Attempting to display textured graphics_object" << std::endl;


  	// enable texturing and bind
  	glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
  	glBindTexture(GL_TEXTURE_2D, tex_handle);

    glBegin(GL_TRIANGLES);
      int n = tex_indices.size();
      int m = 0;
      //std::cout << "Number of vertex indices: " << n << std::endl;
      for (int i = 0; i < n; i++) {
        point3f &vertex = vertices[vertex_indices[i]];
        point2f &tex = tex_coords[tex_indices[i]];
        /*std::cout << "vertex: " << m++
        << "\tindex: " << i
        << "\tx: " << tex_coords[tex_indices[i]].x
        << "\ty: " << tex_coords[tex_indices[i]].y << std::endl;
        //  << "\tz: " << tex_coords[tex_indices[i]].z << std::endl;
        */

        glTexCoord2f(tex.x, tex.y);
        glVertex3f(vertex.x, vertex.y, vertex.z);
        //std::cout << vertex.x << "\t" << vertex.y << "\t" << vertex.z << "\t" << tex.x << "\t" << tex.y << std::endl;
      }

    glEnd();

  	glDisable(GL_TEXTURE_2D);

    //std::cout << "Finished displaying textured graphics_object" << std::endl;

  }

private:
  //Vertex list
  //Vertex indices corresponding to triangles making up the model:
  //First triangle - (vertices[0], vertices[1], vertices[2]), etc
  std::vector<point3f> vertices;
  std::vector<point2f> tex_coords;
  std::vector<int> vertex_indices;
  std::vector<int> tex_indices;
  unsigned int tex_handle;
};

#endif
