#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H


#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"
#include "game_component.h"

class game_object {
public:
  game_object() {
    position = *new point3f();
    scale = *new point3f(1.0f, 1.0f, 1.0f);
    rotation_axis = *new point3f(0.0f, 1.0f, 0.0f);
    velocity = *new point3f();
    rotation_angle = 0.0f;
    parent = NULL;
    g_component = NULL;
  }
  game_object(point3f *position) {
    this->position = *position;
    scale = *new point3f(1.0f, 1.0f, 1.0f);
    rotation_axis = *new point3f(0.0f, 1.0f, 0.0f);
    rotation_angle = 0.0f;
    parent = NULL;
    g_component = NULL;
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

    g_component = NULL;
    //children = std::vector<game_object*>();
  }
  void set_position(point3f *position) {
    this-> position = *position;
  }
  void set_velocity(point3f *velocity) {
    this-> velocity = *velocity;
  }
  void update(float time) {
    this->position = this->position + (this->velocity * time);
    for (game_object* g : children) {
      if (g == NULL) {
        std::cout << "null child" << std::endl;
      } else {
        //std::cout << "displaying child " << g << std::endl;
        g->update(time);
      }
    }
  }
  void display() {
    glPushMatrix();
      //std::cout << "Displaying game_object" << std::endl;

      glScalef(scale.x, scale.y, scale.z);
      glRotatef(rotation_angle, rotation_axis.x, rotation_axis.y, rotation_axis.z);
      glTranslatef(position.x, position.y, position.z);

      //std::cout << "Attempting to iterate through children" << std::endl;
      //std::cout << "number of children: " << children.size() << std::endl;
      for (game_object* g : children) {
        if (g == NULL) {
          std::cout << "null child" << std::endl;
        } else {
          //std::cout << "displaying child " << g << std::endl;
          g->display();
        }
      }

      //std::cout << "done" << std::endl;

      if (g_component == NULL) {
        //std::cout << "g_object is null" << std::endl;
      } else {
        //std::cout << "attempting to draw the graphics object of this game_object" << std::endl;
        g_component->display();

      }
    glPopMatrix();

    //std::cout << "Finished displaying game_object" << std::endl;
  }
  void set_game_component(game_component *g_component) {
    std::cout << "Setting game_component of object" << std::endl;
    this->g_component = g_component;
  }
  game_object* add_child(game_object *child) {
    std::cout << "Adding child to tree" << std::endl;
    children.push_back(child);
    return child;
  }
  void set_parent(game_object *parent) {
    this-> parent = parent;
  }
private:
    game_object* parent;
    std::vector<game_object*> children;
    game_component* g_component;

    point3f position;
    point3f scale;
    point3f rotation_axis;
    point3f velocity;
    float rotation_angle;
};

/* header file
class game_object {

public:
  game_object() {};
  game_object(point3f *position) {};
  game_object(point3f *position, point3f *scale, point3f *rotation_axis, float rotation_angle, game_object *parent) {};
  void set_position(point3f *position) {};
  void display() {};
  void update(int t) {};
  void set_game_component(game_component *g_component) {};
  game_object* add_child(game_object *child) {};
  void set_parent(game_object *parent) {};

private:
  game_object* parent;
  std::vector<game_object*> children;
  game_component* g_component;

  point3f position;
  point3f scale;
  point3f rotation_axis;
  float rotation_angle;

};
*/
#endif
