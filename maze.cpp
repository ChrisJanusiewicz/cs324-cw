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
#include "textured_graphics_object.h"

#include <vector>
#include <memory>

bool g_spinning = false;
int g_spin = 0;

const int NUM_SIDES = 5;
const unsigned int DEFAULT_COLOR = 0xFFFF00FF; //ARGB

std::unique_ptr<game_object> root;

  float mat_ambient[] = {0.3, 0.3, 0.3, 1.0};
  float mat_diffuse[] = {0.75, 0.75, 0.75, 1.0};
  float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  float mat_shininess[] = {50.0};

  float light_position[] = {0.0f, 5.0f, 0.0f, 1.0f};

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

unsigned int g_tex_handle_floor;
unsigned int g_tex_handle_wall;




void load_and_bind_textures() {
	// load all textures here
	g_tex_handle_floor = load_and_bind_texture("images/floor.png");
	g_tex_handle_wall = load_and_bind_texture("images/wall.png");


}

void idle()
{
  usleep(15000); // in microseconds
	g_spin += 2;
	glutPostRedisplay();
}

void display()  {
  std::cout << "drawing..." << std::endl;


  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // position and orient camera
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(3, 1.5, 6, // eye position
        0, 0, 0, // reference point
        0, 1, 0  // up vector
    );


  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glColor3f(1.0f, 0.8f, 1.0f);


    glEnable(GL_LIGHTING);
  //glutWireCube(0.5);
  root->display();

  glDisable(GL_LIGHTING);

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
	gluPerspective(40.0, 1.0f, 1.0, 50.0);

	glutPostRedisplay();
}



void init() {
	load_and_bind_textures();


  float light_ambient[] = {0.1, 0.1, 0.1, 1.0};
  float light_diffuse[] = {0.8, 0.8, 0.8, 1.0};
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

  glShadeModel(GL_SMOOTH);

  // fix the light position
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  // enable lighting and turn on the light0
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);



  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


  //prepare root object
  root.reset(new game_object(new point3f(),
    new point3f(1.0f, 1.0f, 1.0f),
    new point3f(0.0f, 1.0f, 0.0f),
    0.0f,
    NULL));



  glMatrixMode(GL_PROJECTION);
  gluPerspective(40.0, 1.0f, 1.0, 5.0);


  //prepare test graphical object
  //initialise vector to hold vertex information
  std::vector<point3f> vertices;
  //bottom of wall cuboid
  vertices.push_back(*new point3f(-0.5f, 0.0f, -0.5f));
  vertices.push_back(*new point3f(-0.5f, 0.0f, 0.5f));
  vertices.push_back(*new point3f(0.5f, 0.0f, 0.5f));
  vertices.push_back(*new point3f(0.5f, 0.0f, -0.5f));
  //top of the wall cuboid
  vertices.push_back(*new point3f(-0.5f, 2.0f, -0.5f));
  vertices.push_back(*new point3f(-0.5f, 2.0f, 0.5f));
  vertices.push_back(*new point3f(0.5f, 2.0f, 0.5f));
  vertices.push_back(*new point3f(0.5f, 2.0f, -0.5f));

  std::vector<int> vertex_indices{0, 4, 3, 3, 4, 7,
  2, 3, 7, 2, 7, 6,
  1, 2, 6, 1, 6, 5,
  0, 1, 4, 4, 1, 5,
  4, 5, 6, 4, 6, 7};

  std::vector<point2f> tex_coords;
  tex_coords.push_back(*new point2f(0.0f, 0.0f));
  tex_coords.push_back(*new point2f(63.0f, 0.0f));
  tex_coords.push_back(*new point2f(64.0f, 0.0f));
  tex_coords.push_back(*new point2f(127.0f, 0.0f));
  tex_coords.push_back(*new point2f(128.0f, 0.0f));
  tex_coords.push_back(*new point2f(191.0f, 0.0f));
  tex_coords.push_back(*new point2f(192.0f, 0.0f));
  tex_coords.push_back(*new point2f(255.0f, 0.0f));
  tex_coords.push_back(*new point2f(256.0f, 0.0f));
  tex_coords.push_back(*new point2f(319.0f, 0.0f));

  tex_coords.push_back(*new point2f(0.0f, 127.0f));
  tex_coords.push_back(*new point2f(63.0f, 127.0f));
  tex_coords.push_back(*new point2f(64.0f, 127.0f));
  tex_coords.push_back(*new point2f(127.0f, 127.0f));
  tex_coords.push_back(*new point2f(128.0f, 127.0f));
  tex_coords.push_back(*new point2f(191.0f, 127.0f));
  tex_coords.push_back(*new point2f(192.0f, 127.0f));
  tex_coords.push_back(*new point2f(256.0f, 127.0f));
  tex_coords.push_back(*new point2f(255.0f, 63.0f));
  tex_coords.push_back(*new point2f(319.0f, 63.0f));

  std::vector<int> tex_indices{10, 0, 11, 11, 0, 1,
  13, 12, 2, 13, 2, 3,
  15, 14, 4, 15, 4, 5,
  17, 16, 8, 7, 16, 6,
  8, 9, 18, 18, 19, 9};


  int color = 0xFFFF00FF;
  graphics_object *g_object = new graphics_object(&vertices, &vertex_indices, color);
  textured_graphics_object *t_object =
    new textured_graphics_object(&vertices,
      &vertex_indices,
      &tex_coords,
      &tex_indices,
      g_tex_handle_wall);


  root->set_graphics_object(g_object);


  game_object *wall2 = new game_object(new point3f(0.0f, 0.0f, 1.0f),
    new point3f(1.0f, 1.0f, 1.0f),
    new point3f(0.0f, 1.0f, 0.0f),
    45.0f,
    NULL);

  game_object *wall3 = new game_object(new point3f(-1.0f, 0.0f, 2.0f),
    new point3f(1.0f, 1.0f, 1.0f),
    new point3f(0.0f, 1.0f, 0.0f),
    12.0f,
    NULL);

  wall2->set_graphics_object(g_object);
  root->add_child(wall2);
  wall3->set_graphics_object(g_object);
  root->add_child(wall3);


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
