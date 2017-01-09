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

bool g_spinning = false;
int g_spin = 0;

const int NUM_SIDES = 5;

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
	gluLookAt(0, 0.25, 2.5, // eye position
			  0, 0, 0.0, // reference point
			  0, 1, 0  // up vector
		);

	glPushMatrix();
		glRotatef(g_spin, 0, 1, 0);

		glEnable(GL_TEXTURE_2D);

		// map the road surface
		glBindTexture(GL_TEXTURE_2D, g_tex_handle_floor);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-0.5f, -0.5f, 0.5f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(0.5f, -0.5f, 0.5f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(0.5f, -0.5f, -0.5f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-0.5f, -0.5f, -0.5f);
		glEnd();

		// do other textures here
		// do each side
		for (size_t s=0;s<NUM_SIDES;s++)
		{
			glBindTexture(GL_TEXTURE_2D, g_tex_handle_wall[s]);
			glBegin(GL_QUADS);
				for (size_t i=0;i<4;i++)
				{
					glTexCoord2f(
						(g_wall_source_coords[s][i][0]/547.0f) ,
						(411-g_wall_source_coords[s][i][1])/411.0f
					);
					glVertex3fv(g_wall_vertices[s][i]);
				}
			glEnd();
		}

		glDisable(GL_TEXTURE_2D);

		// draw the edges in wire frame
		glPushAttrib(GL_CURRENT_BIT);
			glColor3f(1.0f, 1.0f, 0.0f);
			for (size_t s=0;s<NUM_SIDES;s++)
			{
				glBegin(GL_LINE_LOOP);
				for (size_t v=0;v<4;v++)
					glVertex3fv(g_wall_vertices[s][v]);
				glEnd();
			}
		glPopAttrib();


	glPopMatrix();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int, int)
{
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

void reshape(int w, int h)
{
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

void init()
{
  prepare_wall_vertices();
	load_and_bind_textures();

	GLenum error = glGetError();
	if (error!=GL_NO_ERROR)
		fprintf(stderr, "GL error %s\n", gluErrorString(error));


	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("VW Camper Van");

	// get texture ready before we need it
	init();

	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}
