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
	float direction[4];
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
    glLightf(light.name, GL_QUADRATIC_ATTENUATION, 1);

		glLightf(light.name, GL_LINEAR_ATTENUATION, 0);
		glLightf(light.name, GL_CONSTANT_ATTENUATION, 0);
  	// make the lights spot lights here
  	//glLightfv(light.name, GL_SPOT_DIRECTION, light.direction);
  	//glLightf(light.name, GL_SPOT_CUTOFF, 100.0f);

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
