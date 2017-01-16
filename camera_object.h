#ifndef CAMERA_OBJECT_H
#define CAMERA_OBJECT_H

#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"
#include "game_component.h"
#include "cross_product.h"

class camera_object : public game_component {
public:

    camera_object () {
        up_vector = *new point3f(0, 1, 0);
    }
    virtual void update(float time) {


    }
    void look() {
        //std::cout << "calling gluLookAt" << std::endl;
        gluLookAt(position.x, position.y, position.z, // eye position
                  position.x + direction.x, position.y + direction.y, position.z + direction.z, // reference point
                  0.0f, 1.0f, 0.0f  // up vector
                 );
    }
    virtual void set_position(point3f *position) {
        this->position = *position;
    }
    virtual void set_direction(point3f * direction) {
        this->direction = *direction;
    }

    virtual void display() {

        look();
    }

private:
    point3f position;
    point3f direction;
    point3f up_vector;
};

#endif
