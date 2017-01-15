#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

//Class will contain graphics objects without textures
#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"

class game_component {
public:
  game_component () {}
  virtual void update() {}
  virtual void display() {}
private:
};

#endif
