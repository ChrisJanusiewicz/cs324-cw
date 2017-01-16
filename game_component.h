#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

//Class can be extended by others such as graphics_component, camera_object,
#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"

class game_component {
public:
    game_component () {}
    virtual void update() {}
    virtual void display() {
        //std::cout << "running base update() of game_component" << std::endl;
    }
private:
};

#endif
