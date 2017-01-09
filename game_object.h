#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H


#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"
#include "graphics_object.h"


class game_object {

public:

  game_object() {
    parent = NULL;
  }

  game_object(point3f *position,
    point3f *rotation,
    point3f *scale,
    game_object *parent) {
    this->position = *position;
    this->rotation = *rotation;
    this->scale = *scale;
    this->parent = parent;
    //children = std::vector<game_object*>();
  }

  void display() {
    std::cout << "Attempting to display game_object" << std::endl;
    for (game_object* g : children) {
      std::cout << g << std::endl;
      g->display();

    }
    if (g_object != NULL) {
      g_object->display();
    }

    std::cout << "Finished displaying game_object" << std::endl;
  }

  void update(int t) {


  }

  void set_graphics_object(graphics_object *g_object) {
    this->g_object = g_object;
  }
  


private:
  game_object* parent;
  std::vector<game_object*> children;
  graphics_object* g_object;

  point3f position;
  point3f rotation;
  point3f scale;

};
#endif
