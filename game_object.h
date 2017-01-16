#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H


#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"
#include "game_component.h"

//The class that makes up the display tree. Holds game_component which can be
//inherited by graphics_object, textured_graphics_object, etc
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

    void set_position(point3f *position) {
        this-> position = *position;
    }
    void set_scale(point3f *scale) {
        this-> scale = *scale;
    }
    void set_velocity(point3f *velocity) {
        this-> velocity = *velocity;
    }
    //Used to update its own position and its children (resursive)
    void update(float time) {
        this->position = this->position + (this->velocity * time);
        for (game_object* g : children) {
            if (g == NULL) {
                std::cout << "null child" << std::endl;
            } else {
                g->update(time);
            }
        }
    }
    //Recursive: displays itself, then all its children.
    void display() {
      //Push matrix, and apply correct transformations, so that they can be reversed with popmatrix()
        glPushMatrix();
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


        if (g_component == NULL) {
        } else {
            //std::cout << "attempting to draw the graphics object of this game_object" << std::endl;
            g_component->display();
        }
        glPopMatrix();
    }
    void set_game_component(game_component *g_component) {
        //std::cout << "Setting game_component of object" << std::endl;
        this->g_component = g_component;
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
    game_component* g_component;

    point3f position;
    point3f scale;
    point3f rotation_axis;
    point3f velocity;
    float rotation_angle;
};

#endif
