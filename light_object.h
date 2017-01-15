#ifndef LIGHT_OBJECT_H
#define LIGHT_OBJECT_H

//Class will contain graphics objects without textures
#include <stdlib.h>
#include <stddef.h>
#include <vector>
#include "point3f.h"
#include "game_component.h"

// light struct
class light_t
{
public:
	unsigned int name;
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float position[4];
};

class light_object : public game_component {
public:

  light_object (light_t light_info) {
    light = light_info;
    //std::cout << "light name from constructor: " << light_info.name << std::endl;
    //std::cout << "light name from member: " << light.name << std::endl;
    //std::cout << GL_LIGHT1 << std::endl;
  }
  void update() {

  }
  void disable() {
    glDisable(light.name);
  }
  void enable() {
    glEnable(light.name);
  }
  void display() {
    //std::cout << "display() of light_object was called: " << light.name << std::endl;
    //std::cout << GL_LIGHT1 << std::endl;
    //float rand_x = float(drand48() * 5);
    //float light_position[] = {rand_x, 0, 0, 1};



    glLightfv(light.name, GL_AMBIENT, light.ambient);
    glLightfv(light.name, GL_DIFFUSE, light.diffuse);
    glLightfv(light.name, GL_SPECULAR, light.specular);
    glLightfv(light.name, GL_POSITION, light.position);

    /*
    std::cout << light->position[0] << " "
    << light->position[1] << " "
    << light->position[2] << std::endl;
    std::cout << light->ambient[0] << " "
    << light->ambient[1] << " "
    << light->ambient[2] << std::endl;
      */

/*
        float light_ambient[] = {0, 0, 1, 1};
        float light_diffuse[] = {0, 0, 0, 1.0};
        float light_position[] = {0, 1.0, -20.0, 0.0};
        float light_direction[] = {0.0, 0.0, 1.0, 1.0};
        glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
        glEnable(GL_LIGHT2);
*/

    /*
  	// make the lights spot lights here
  	float direction[3] = {
  				-light->position[0],
  				-light->position[1],
  				-light->position[2]};
  	glLightfv(light->name, GL_SPOT_DIRECTION, direction);
  	glLightf(light->name, GL_SPOT_CUTOFF, 100.0f);
    */

    //glEnable(light->name);
    //std::cout << "enabling: " << light->name << std::endl;

  }

private:
  light_t light;
	/*size_t name;
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float position[4];*/
};

#endif
