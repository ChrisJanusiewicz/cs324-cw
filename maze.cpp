//Copyright (c) Copyright Holder All Rights Reserved.
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <iostream>
#include <math.h>
#include <chrono> // for high_resolution_clock

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h> // for usleep

#include "load_and_bind_texture.h"
#include "game_object.h"
#include "graphics_object.h"
#include "textured_graphics_object.h"
#include "draw_text.h"

#include <vector>
#include <memory>

bool g_spinning = false;
int g_spin = 0;

float angle = 0.0f;
const int NUM_SIDES = 5;
const unsigned int DEFAULT_COLOR = 0xFFFF00FF; //ARGB

std::unique_ptr<game_object> root;

float mat_ambient[] = {0.3, 0.3, 0.3, 1.0};
float mat_diffuse[] = {0.75, 0.75, 0.75, 1.0};
float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
float mat_shininess[] = {50.0};

float light_position[] = {0.0f, 5.0f, 0.0f, 1.0f};


//Camera information
point3f camera_position;
point3f camera_direction;
float camera_angle_y;
float camera_speed = 3.0f;
float camera_turning_speed = 90.0f;

//time information
auto last_time = std::chrono::high_resolution_clock::now();

//mouse information

//keyboard information
bool key_down_a;
bool key_down_s;
bool key_down_d;
bool key_down_w;

float fps_max = 60.0f;

unsigned int g_tex_handle_floor;
unsigned int g_tex_handle_wall;


void normalise (point3f *p) {
  float magnitude = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
  p->x = p->x / magnitude;
  p->y = p->y / magnitude;
  p->z = p->z / magnitude;
}
void load_and_bind_textures() {
	// load all textures here
	//g_tex_handle_floor = load_and_bind_texture("images/floor.png");

	//g_tex_handle_wall = load_and_bind_texture("images/ak.png");
	//g_tex_handle_wall = load_and_bind_texture("images/wall.png");
	g_tex_handle_wall = load_and_bind_texture("images/hellrock.png");

}
void idle() {
  auto now = std::chrono::high_resolution_clock::now();
  auto elapsed = now - last_time;
  long long microseconds_elapsed =
    std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    //std::cout << microseconds_elapsed << std::endl;

    float seconds_elapsed = microseconds_elapsed / 1000000.0f;
    //std::cout << seconds_elapsed << std::endl;

    if (key_down_a) {
        angle += camera_turning_speed * seconds_elapsed;

    }
    if (key_down_d) {
        angle -= -camera_turning_speed * seconds_elapsed;
    }
    if (key_down_w) {
      camera_position = *new point3f(camera_position.x + camera_direction.x * camera_speed * seconds_elapsed,
      camera_position.y + camera_direction.y * camera_speed * seconds_elapsed,
      camera_position.z + camera_direction.z * camera_speed * seconds_elapsed);
    }
    if (key_down_s) {
      camera_position = *new point3f(camera_position.x + camera_direction.x * -camera_speed * seconds_elapsed,
      camera_position.y + camera_direction.y * -camera_speed * seconds_elapsed,
      camera_position.z + camera_direction.z * -camera_speed * seconds_elapsed);
    }

    glutPostRedisplay();
    last_time = now;

  usleep(1000); // in microseconds
}
void update_camera(){
  float old_x, old_z, new_x, new_z;
  old_x = camera_direction.x;
  old_z = camera_direction.z;
  new_x = old_x * cos(angle) + old_z * sin(angle);
  new_z = -old_x * sin(angle) + old_z * cos(angle);
  camera_direction = *new point3f(new_x, 0, new_z);
  normalise(&camera_direction);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //glRotatef(angle, 0.0f, 1.0f, 0.0f);
  gluLookAt(camera_position.x, camera_position.y, camera_position.z, // eye position
        camera_position.x + camera_direction.x, camera_position.y + camera_direction.y, camera_position.z + camera_direction.z, // reference point
        0.0f, 1.0f, 0.0f  // up vector
      );


}
void display() {
  //std::cout << "drawing..." << std::endl;


  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glColor3f(1.0f, 1.0f, 1.0f);;


  // position and orient camera
  update_camera();

  glPushMatrix();
    //glScalef(1.0f, 1.0f, 1.0f);

    root->display();

  glPopMatrix();

    draw_text(0, 900, camera_position.to_string());
    draw_text(0, 850, camera_direction.to_string());
    //draw_text(0, 800, );

  glutSwapBuffers();
}
void keyboard_up(unsigned char key, int, int) {
  switch (key) {
		case 'w': key_down_w = false; break;
		case 'a': key_down_a = false; break;
		case 's': key_down_s = false; break;
		case 'd': key_down_d = false; break;
  }
}
void keyboard_down(unsigned char key, int, int) {
  switch (key) {
    case 'w': key_down_w = true; break;
    case 'a': key_down_a = true; break;
    case 's': key_down_s = true; break;
    case 'd': key_down_d = true; break;
  }
}
void processSpecialKeys(int key, int xx, int yy) {

	float new_x, new_z, old_x, old_z;
  float speed = 0.5;
  float angle_inc = 0.1f;
  angle = 0.0f;
	switch (key) {
		case GLUT_KEY_LEFT :
      angle = angle_inc;
      //std:: cout << "Rotating by: " << angle << std::endl;
			break;
		case GLUT_KEY_RIGHT :
      angle = -angle_inc;
      //std:: cout << "Rotating by: " << angle << std::endl;
      break;

		case GLUT_KEY_UP :
      camera_position = *new point3f(camera_position.x + camera_direction.x * speed,
        camera_position.y + camera_direction.y * speed,
        camera_position.z + camera_direction.z * speed);
			break;
		case GLUT_KEY_DOWN :
      camera_position = *new point3f(camera_position.x + camera_direction.x * -speed,
        camera_position.y + camera_direction.y * -speed,
        camera_position.z + camera_direction.z * -speed);
			break;
	}
  old_x = camera_direction.x;
  old_z = camera_direction.z;
  new_x = old_x * cos(angle) + old_z * sin(angle);
  new_z = -old_x * sin(angle) + old_z * cos(angle);
  camera_direction = *new point3f(new_x, 0, new_z);
  normalise(&camera_direction);

  glutPostRedisplay();
}
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  float ar = w/(float)h;
  std::cout << ar << std::endl;
	gluPerspective(60.0, ar, 0.25f, 50.0);

	glutPostRedisplay();
}
// Returns textured cuboid
graphics_object* prepare_graphics_object(bool texture) {
  //prepare test graphical object
  //initialise vector to hold vertex information
  std::vector<point3f> vertices;
  //bottom of wall cuboid
  vertices.push_back(*new point3f(-1.5f, 0.0f, -1.5f));
  vertices.push_back(*new point3f(-1.5f, 0.0f, 1.5f));
  vertices.push_back(*new point3f(1.5f, 0.0f, 1.5f));
  vertices.push_back(*new point3f(1.5f, 0.0f, -1.5f));
  //top of the wall cuboid
  vertices.push_back(*new point3f(-1.5f, 2.0f, -1.5f));
  vertices.push_back(*new point3f(-1.5f, 2.0f, 1.5f));
  vertices.push_back(*new point3f(1.5f, 2.0f, 1.5f));
  vertices.push_back(*new point3f(1.5f, 2.0f, -1.5f));


    std::vector<int> vertex_indices{0, 4, 3, 3, 4, 7,
    2, 3, 7, 2, 7, 6,
    1, 2, 6, 1, 6, 5,
    0, 1, 4, 4, 1, 5,
    4, 5, 6, 4, 6, 7};

  //std::vector<int> vertex_indices{0, 1, 2, 0, 2, 3};

  std::vector<point2f> tex_coords;
  tex_coords.push_back(*new point2f(0.0f, 0.0f));
  tex_coords.push_back(*new point2f(0.2f, 0.0f));
  tex_coords.push_back(*new point2f(0.2f, 0.0f));
  tex_coords.push_back(*new point2f(0.4f, 0.0f));
  tex_coords.push_back(*new point2f(0.4f, 0.0f));
  tex_coords.push_back(*new point2f(0.6f, 0.0f));
  tex_coords.push_back(*new point2f(0.6f, 0.0f));
  tex_coords.push_back(*new point2f(0.8f, 0.0f));
  tex_coords.push_back(*new point2f(0.8f, 0.5f));
  tex_coords.push_back(*new point2f(1.0f, 0.5f));

  tex_coords.push_back(*new point2f(0.0f, 1.0f));
  tex_coords.push_back(*new point2f(0.2f, 1.0f));
  tex_coords.push_back(*new point2f(0.2f, 1.0f));
  tex_coords.push_back(*new point2f(0.4f, 1.0f));
  tex_coords.push_back(*new point2f(0.4f, 1.0f));
  tex_coords.push_back(*new point2f(0.6f, 1.0f));
  tex_coords.push_back(*new point2f(0.6f, 1.0f));
  tex_coords.push_back(*new point2f(0.8f, 1.0f));
  tex_coords.push_back(*new point2f(0.8f, 0.5f));
  tex_coords.push_back(*new point2f(1.0f, 0.5f));
  /*
    tex_coords.push_back(*new point2f(0.0f, 0.0f));
    tex_coords.push_back(*new point2f(1.0f, 0.0f));
    tex_coords.push_back(*new point2f(1.0f, 1.0f));
    tex_coords.push_back(*new point2f(0.0f, 1.0f));
  */
  std::vector<int> tex_indices{10, 0, 11, 11, 0, 1,
  13, 12, 2, 13, 2, 3,
  15, 14, 4, 15, 4, 5,
  17, 16, 8, 7, 16, 6,
  8, 9, 18, 18, 19, 9};


  //std::vector<int> tex_indices{0, 2, 3, 0, 2, 3};

  int color = 0xFFFF00FF;
  graphics_object *g_object = new graphics_object(&vertices, &vertex_indices, color);
  textured_graphics_object *t_object = new textured_graphics_object(&vertices, &vertex_indices, &tex_coords, &tex_indices, &g_tex_handle_wall);

  if (texture) {
    return t_object;
  } else {
    return g_object;
  }

}
void prepare_maze(game_object *root) {

  graphics_object *wall = prepare_graphics_object(true);

  int maze[][7] = {{0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1},
    {0,1,0,0,0,0,0},
    {0,1,0,1,1,1,0},
    {0,1,0,1,0,0,0},
    {0,1,1,1,1,1,0},
    {0,0,0,0,0,1,0}};

    point3f scale = *new point3f(1.0f, 1.0f, 1.0f);
    point3f rotation = *new point3f(0.0f, 1.0f, 0.0f);



    for (int x = 0; x < 7; x++) {
      for (int y = 0; y < 7; y++) {
        if (maze[x][y] == 0) {
          game_object* g = new game_object(new point3f(3 * x-3, 0.0f, 3 * y-3));
          //std::cout << g << std::endl;
          g->set_graphics_object(wall);
          root->add_child(g);
        }
      }
    }


}
void init() {
	load_and_bind_textures();

  /*
  //float light_ambient[] = {0.1, 0.1, 0.1, 1.0};
  //float light_diffuse[] = {0.8, 0.8, 0.8, 1.0};
  //glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  //glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

  //glShadeModel(GL_SMOOTH);

  // fix the light position
  //glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  // enable lighting and turn on the light0
  //glEnable(GL_LIGHTING);
  //glEnable(GL_LIGHT0);

  //glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  */
  //prepare root object
  root.reset(new game_object());

  prepare_maze(&*root);
  //root->set_graphics_object(prepare_graphics_object(true));

  camera_position = *new point3f(0.0f, 1.5f, -5.0f);
  camera_direction = *new point3f(0.0f, 0.0f, 1.0f);

	GLenum error = glGetError();
	if (error!=GL_NO_ERROR) {
		fprintf(stderr, "GL error %s\n", gluErrorString(error));
  }

	glEnable(GL_DEPTH_TEST);
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Maze");

  glutIdleFunc(idle);

	glutKeyboardFunc(keyboard_down);
	glutKeyboardUpFunc(keyboard_up);
  glutSpecialFunc(processSpecialKeys);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);



	// get texture ready before we need it
	init();

	glutMainLoop();

	return 0;
}
