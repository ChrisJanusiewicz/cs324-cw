#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H


#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"
#include "graphics_object.h"


class game_object {


  // properties of default material


public:

  game_object() {
    position = *new point3f();
    scale = *new point3f(1.0f, 1.0f, 1.0f);
    rotation_axis = *new point3f(0.0f, 1.0f, 0.0f);
    rotation_angle = 0.0f;
    parent = NULL;
    g_object = NULL;
  }

  game_object(point3f *position) {

    this->position = *position;
    scale = *new point3f(1.0f, 1.0f, 1.0f);
    rotation_axis = *new point3f(0.0f, 1.0f, 0.0f);
    rotation_angle = 0.0f;
    parent = NULL;
    g_object = NULL;

  }

  game_object(point3f *position,
    point3f *scale,
    point3f *rotation_axis,
    float rotation_angle,
    game_object *parent) {

    this->position = *position;
    this->rotation_axis = *rotation_axis;
    this->scale = *scale;
    this->parent = parent;

    g_object = NULL;
    //children = std::vector<game_object*>();

  }

  void set_position(point3f *position) {
    this-> position = *position;

  }


  void display() {
    //std::cout << "Attempting to display game_object" << std::endl;

    glPushMatrix();

      //std::cout << "Scaling by: " << scale.x << " " << scale.y << " " << scale.z << std::endl;
      //std::cout << "Rotating by: " <<rotation_angle << " around:"
      //<< rotation_axis.x << " "
      //<< rotation_axis.y << " "
      //<< rotation_axis.z << std::endl;
      //std::cout << "Translating by: " << position.x << " " << position.y << " " << position.z << std::endl;

      glScalef(scale.x, scale.y, scale.z);
      glRotatef(rotation_angle, rotation_axis.x, rotation_axis.y, rotation_axis.z);
      glTranslatef(position.x, position.y, position.z);


      //std::cout << "Attempting to iterate through children" << std::endl;
      for (game_object* g : children) {
        if (g == NULL) {
          std::cout << "null child" << std::endl;
        } else {
          //std::cout << g << std::endl;
          g->display();
        }
      }

      //std::cout << "done" << std::endl;

      if (g_object == NULL) {
        //std::cout << "g_object is null" << std::endl;
      } else {
        //std::cout << "attempting to draw the graphics object of this game_object" << std::endl;
        g_object->display();

      }


    glPopMatrix();

    //std::cout << "Finished displaying game_object" << std::endl;
  }

  void update(int t) {


  }

  void set_graphics_object(graphics_object *g_object) {
    this->g_object = g_object;
  }

  game_object* add_child(game_object *child) {

    //std::cout << "Adding child to tree" << std::endl;
    children.push_back(child);
    return child;

  }

  void set_parent(game_object *parent) {
    this-> parent = parent;
  }


private:
  game_object* parent;
  std::vector<game_object*> children;
  graphics_object* g_object;

  point3f position;
  point3f scale;
  point3f rotation_axis;
  float rotation_angle;

};
#endif
