//Copyright (c) Copyright Holder All Rights Reserved.
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <math.h>
#include <chrono> // for high_resolution_clock

#include <vector>
#include <memory>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h> // for usleep

#include "load_and_bind_texture.h"
#include "game_object.h"
#include "graphics_object.h"
#include "textured_graphics_object.h"
#include "light_object.h"
#include "draw_text.h"

float angle = 0.0f;

std::unique_ptr<game_object> root;
std::unique_ptr<game_object> player_torch_object;

float light_position[] = {0.0f, 5.0f, 0.0f, 1.0f};

//Camera information
point3f camera_position;
point3f camera_direction;

float camera_speed = 10.0f;
float camera_turning_speed = 1.67f;

//time information
auto last_time = std::chrono::high_resolution_clock::now();

light_t player_torch_light = {
    GL_LIGHT1,
    {1, 1, 0, 1},
    {0.5, 0.5, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 1}
};


//keyboard information
bool key_down_a;
bool key_down_s;
bool key_down_d;
bool key_down_w;

unsigned int g_tex_handle_floor;
unsigned int g_tex_handle_wall;


void load_and_bind_textures() {
    // load all textures here
    g_tex_handle_floor = load_and_bind_texture("images/floor.png");
    g_tex_handle_wall = load_and_bind_texture("images/hellrock.png");
}
void idle() {
    //calculate time passed since last time this routine was executed

    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = now - last_time;
    long long microseconds_elapsed =
        std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    float seconds_elapsed = microseconds_elapsed / 1000000.0f;

    //UPDATE CAMERA
    if (key_down_a) {
        angle = camera_turning_speed * seconds_elapsed;
    } else if (key_down_d) {
        angle = -camera_turning_speed * seconds_elapsed;
    } else {
        angle = 0.0f;
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

    //UPDATE OBJECT tree
    root->update(seconds_elapsed);

    //redraw
    glutPostRedisplay();
    last_time = now;

    usleep(15000); // in microseconds
}

void update_camera() {
    //get correct direction and position vectors by rotating around the y axis
    float old_x, old_z, new_x, new_z;
    old_x = camera_direction.x;
    old_z = camera_direction.z;
    new_x = old_x * cos(angle) + old_z * sin(angle);
    new_z = -old_x * sin(angle) + old_z * cos(angle);
    camera_direction = *new point3f(new_x, 0, new_z);
    normalise(&camera_direction);

    //move the player light to the camera's position
    player_torch_object->set_position(new point3f(camera_position.x, camera_position.y, camera_position.z));
    gluLookAt(camera_position.x, camera_position.y, camera_position.z, // eye position
              camera_position.x + camera_direction.x, camera_position.y + camera_direction.y, camera_position.z + camera_direction.z, // reference point
              0.0f, 1.0f, 0.0f  // up vector
             );

}
void display() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1,1,1);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // position and orient camera
    update_camera();

    //std::cout << "Displaying root object..." << std::endl;
    root->display();

    glutSwapBuffers();
}
void keyboard_up(unsigned char key, int, int) {
    switch (key) {
    case 'w':
        key_down_w = false;
        break;
    case 'a':
        key_down_a = false;
        break;
    case 's':
        key_down_s = false;
        break;
    case 'd':
        key_down_d = false;
        break;
    }
}
void keyboard_down(unsigned char key, int, int) {
    switch (key) {
    case 'w':
        key_down_w = true;
        break;
    case 'a':
        key_down_a = true;
        break;
    case 's':
        key_down_s = true;
        break;
    case 'd':
        key_down_d = true;
        break;
    }
}

//happens whenever the viewport is resized
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //aspect ratio
    float ar = w/(float)h;
    std::cout << ar << std::endl;
    gluPerspective(60.0, ar, 0.25f, 50.0);

    glutPostRedisplay();
}
graphics_object* prepare_graphics_object(bool texture) {
    // Returns textured cuboid

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


    std::vector<int> vertex_indices {0, 4, 3, 3, 4, 7,
                                     2, 3, 7, 2, 7, 6,
                                     1, 2, 6, 1, 6, 5,
                                     0, 1, 4, 4, 1, 5,
                                     4, 5, 6, 4, 6, 7
                                    };

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
    std::vector<int> tex_indices {10, 0, 11, 11, 0, 1,
                                  13, 12, 2, 13, 2, 3,
                                  15, 14, 4, 15, 4, 5,
                                  17, 16, 8, 7, 16, 6,
                                  8, 9, 18, 18, 19, 9
                                 };


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
graphics_object* prepare_torch_graphics_object() {
    // Returns torch model

    //initialise vector to hold vertex information
    std::vector<point3f> vertices;
    //wall end of horizontal stick that attaches torch to wall
    vertices.push_back(*new point3f(-0.1, +0.1, 0));
    vertices.push_back(*new point3f(+0.1, +0.1, 0));
    vertices.push_back(*new point3f(-0.1, -0.1, 0));
    vertices.push_back(*new point3f(+0.1, -0.1, 0));
    //torch end of horizontal stick that attaches torch to wall
    vertices.push_back(*new point3f(-0.1, +0.1, 0.4));
    vertices.push_back(*new point3f(+0.1, +0.1, 0.4));
    vertices.push_back(*new point3f(-0.1, -0.1, 0.25f));
    vertices.push_back(*new point3f(+0.1, -0.1, 0.25f));
    //top end of torch
    vertices.push_back(*new point3f(-0.1, +0.4, 0.5));
    vertices.push_back(*new point3f(+0.1, +0.3, 0.6));
    vertices.push_back(*new point3f(-0.1, +0.3, 0.6));
    vertices.push_back(*new point3f(+0.1, +0.4, 0.5));
    //bottom end of torch
    vertices.push_back(*new point3f(-0.1, -0.3, 0));
    vertices.push_back(*new point3f(+0.1, -0.3, 0));
    vertices.push_back(*new point3f(-0.1, -0.4, 0));
    vertices.push_back(*new point3f(+0.1, -0.4, 0));


    std::vector<int> vertex_indices {
        0, 2, 1, 1, 2, 3,
        4, 5, 6, 1, 6, 5,
        0, 1, 4, 4, 1, 5,
        3, 2, 7, 7, 2, 6,
        2, 0, 6, 6, 0, 7,
        1, 3, 5, 5, 3, 7,
        8, 9,10,10, 9,11,
        14,12,8,14, 8,10,
        15,14,10,15,10,11,
        12,14,13,14,15,13,
        8,13,9,8,12,13,
        9,13,15,9,15,11
    };

    int color = 0x654321;
    graphics_object *g_object = new graphics_object(&vertices, &vertex_indices, color);

    return g_object;

}
graphics_object* prepare_floor() {
  // Returns textured cuboid

  //prepare test graphical object
  //initialise vector to hold vertex information
  std::vector<point3f> vertices;
  //bottom of wall cuboid
  vertices.push_back(*new point3f(-1.5f, 0.0f, -1.5f));
  vertices.push_back(*new point3f(-1.5f, 0.0f, 1.5f));
  vertices.push_back(*new point3f(1.5f, 0.0f, 1.5f));
  vertices.push_back(*new point3f(1.5f, 0.0f, -1.5f));
  //top of the wall cuboid


  std::vector<int> vertex_indices {0, 1, 2, 2, 3, 0};

  //std::vector<int> vertex_indices{0, 1, 2, 0, 2, 3};

  std::vector<point2f> tex_coords;
  tex_coords.push_back(*new point2f(0.0f, 0.0f));
  tex_coords.push_back(*new point2f(0.0f, 1.0f));
  tex_coords.push_back(*new point2f(1.0f, 1.0f));
  tex_coords.push_back(*new point2f(1.0f, 0.0f));
  std::vector<int> tex_indices {0, 1, 2, 2, 3, 0};

  textured_graphics_object *t_object = new textured_graphics_object(&vertices, &vertex_indices, &tex_coords, &tex_indices, &g_tex_handle_floor);

  return t_object;

}
void prepare_maze(game_object *root) {

    //prepare the three 3D models
    graphics_object *g_wall = prepare_graphics_object(true);
    graphics_object *g_torch = prepare_torch_graphics_object();
    graphics_object *g_floor = prepare_floor();

    //layout of the maze
    int maze[][11] = {
        {0,1,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,1,1,1,1,0,1,0},
        {0,1,0,1,0,1,0,1,2,1,0},
        {0,1,1,1,0,1,0,1,1,1,0},
        {0,0,0,0,0,1,0,0,0,0,0},
        {0,1,1,1,1,1,1,1,1,1,0},
        {0,1,0,0,0,0,0,0,0,1,0},
        {0,1,0,1,1,1,0,1,0,1,0},
        {0,1,0,1,2,1,0,1,0,1,0},
        {0,1,1,1,0,1,0,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0}
    };

    for (int x = 0; x < 11; x++) {
        for (int y = 0; y < 11; y++) {
          game_object* g = new game_object(new point3f(3 * x-6, 0.0f, 3 * y-6));
            if (maze[x][y] == 1) {
                //no wall. Set floor
                g->set_game_component(g_floor);
            } else {
                //there is a wall here
                g->set_game_component(g_wall);

                if (maze[x][y] == 2) {
                    //add a torch to the wall.
                    game_object* gt = new game_object(new point3f(0, 1.5f, 1.5));
                    game_object* gtl = new game_object(new point3f(0,0,0));
                    gt->set_game_component(g_torch);
                    g->add_child(gt);
                }
            }
            root->add_child(g);
        }
    }

    player_torch_object->set_game_component(new light_object(player_torch_light));
    glEnable(GL_LIGHT1);

    root->add_child(&*player_torch_object);
    game_object *torch1 = new game_object(new point3f(0, 0,0));
    torch1->set_game_component(g_torch);
    root->add_child(torch1);

}
void init() {
    load_and_bind_textures();

    // enable lighting and turn on the light0
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_MODELVIEW);
    float light_ambient[] = {0.01, 0, 0, 1.0};
    float light_diffuse[] = {0, 0, 0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glEnable(GL_LIGHT0);

    glShadeModel(GL_SMOOTH);

    //prepare root object and player camera object
    player_torch_object.reset(new game_object());
    root.reset(new game_object());
    prepare_maze(&*root);

    camera_position = *new point3f(9.0f, 1.5f, 9.0f);
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
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    init();

    glutMainLoop();

    return 0;
}
