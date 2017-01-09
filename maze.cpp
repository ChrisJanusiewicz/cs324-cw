//Copyright (c) Copyright Holder All Rights Reserved.
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <iostream>

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h> // for usleep

#include "load_and_bind_texture.h"
#include "game_object.h"
#include "graphics_object.h"

#include <vector>
#include <memory>

bool g_spinning = false;
int g_spin = 0;

const int NUM_SIDES = 5;
const unsigned int DEFAULT_COLOR = 0xFFFF00FF; //ARGB

std::unique_ptr<game_object> root;

enum wall_sides_t {
  NORTH=0,
  WEST=1,
  SOUTH=2,
  EAST=3,
  TOP=4
};

//Wall element (cuboid) properties
const float g_wall_thickness = 1.0f;
const float g_wall_height = 2.0f;

//coordinates on the input images
const int g_wall_source_coords[NUM_SIDES][4][2] = {
	{{0, 0}, {0, 1}, {1, 1}, {1, 0}},
	{{0, 0}, {0, 1}, {1, 1}, {1, 0}},
	{{0, 0}, {0, 1}, {1, 1}, {1, 0}},
	{{0, 0}, {0, 1}, {1, 1}, {1, 0}},
	{{0, 0}, {0, 1}, {1, 1}, {1, 0}}
};

// target coordinates
float g_wall_vertices[NUM_SIDES][4][3];

// texture handles
unsigned int g_tex_handle_floor = 0;
unsigned int g_tex_handle_wall[NUM_SIDES];

/*struct point3i {
  int x;
  int y;
  int z;

  point3i(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
};*/

/*class g_tex_object : g_object {

  //texture coordinates
  std::vector<point2f> tex_coords;

  //Triangle indices for texture coordinates
  //First triangle will have vertices 0, 1, 2, etc
  std::vector<int> tex_indices;

  //Handle to the texture used for this object
  unsigned int tex_handle;

  g_tex_object(std::vector<point3f> vertices,
    std::vector<point2f> tex_coords,
    std::vector<int> vertex_indices,
    std::vector<int> tex_indices,
    unsigned int tex_handle) {
      g_object(vertices, vertex_indices, DEFAULT_COLOR);

  }

};*/


void load_and_bind_textures()
{
	// load all textures here
	g_tex_handle_floor = load_and_bind_texture("images/floor.png");

	// load the camper images here
	g_tex_handle_wall[NORTH] = load_and_bind_texture("images/wall-north.png");
	g_tex_handle_wall[WEST] = load_and_bind_texture("images/wall-west.png");
	g_tex_handle_wall[SOUTH] = load_and_bind_texture("images/wall-south.png");
	g_tex_handle_wall[EAST] = load_and_bind_texture("images/wall-east.png");
	g_tex_handle_wall[TOP] = load_and_bind_texture("images/wall-top.png");

}

void idle()
{
  usleep(15000); // in microseconds
	g_spin += 2;
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// position and orient camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 1.5f, 10.0f, // eye position
			  0.0f, 0.0f, 0.0f, // reference point
			  0.0f, 1.0f, 0.0f  // up vector
		);

  root->display();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int, int) {
	switch (key)
	{
		case 'q': exit(1); break;

		case ' ':
				g_spinning = !g_spinning;
				if (g_spinning)
					glutIdleFunc(idle);
				else
					glutIdleFunc(NULL);
				break;
	}
	glutPostRedisplay();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 1.0f, 1.0, 5.0);

	glutPostRedisplay();
}

void prepare_wall_vertices() {
  float g_l = -0.5f * g_wall_thickness;
  float g_r = 0.5f * g_wall_thickness;
  float g_k = -0.5f * g_wall_thickness;
  float g_f = 0.5f * g_wall_thickness;
  float g_b = 0.0f;
  float g_t = g_wall_height;
  float vertices[NUM_SIDES][4][3] = {
   	{{g_l, g_b, g_f}, {g_r, g_b, g_f}, {g_r, g_t, g_f}, {g_l, g_t, g_f}}, // front
   	{{g_r, g_b, g_f}, {g_r, g_b, g_k}, {g_r, g_t, g_k}, {g_r, g_t, g_f}}, // left-side
   	{{g_l, g_b, g_k}, {g_l, g_b, g_f}, {g_l, g_t, g_f}, {g_l, g_t, g_k}}, // right-side
   	{{g_r, g_b, g_k}, {g_l, g_b, g_k}, {g_l, g_t, g_k}, {g_r, g_t, g_k}},  // back
   	{{g_r, g_t, g_f}, {g_r, g_t, g_k}, {g_l, g_t, g_k}, {g_l, g_t, g_f}} // top
  };
  std::copy((vertices), (vertices) + NUM_SIDES, (g_wall_vertices));



}

void init() {
  prepare_wall_vertices();
	load_and_bind_textures();

  root.reset(new game_object(new point3f(),
    new point3f(),
    new point3f(1.0f, 1.0f, 1.0f),
    NULL));


	GLenum error = glGetError();
	if (error!=GL_NO_ERROR) {
		fprintf(stderr, "GL error %s\n", gluErrorString(error));
  }

	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Maze");

	// get texture ready before we need it
	init();

	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}
