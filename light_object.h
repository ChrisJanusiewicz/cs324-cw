#ifndef LIGHT_OBJECT_H
#define LIGHT_OBJECT_H

//Class will contain graphics objects without textures
#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"
#include "game_component.h"

int current_light = 1;

class light_object : public game_component {
public:

  light_object (GLenum light, GLenum pname,	GLfloat param) {


  }
  light_object (GLenum pname, GLfloat param) {

  }
  light_object () {

  }
  void update() {

  }
  void display() {
    //
  }

private:

};

#endif
