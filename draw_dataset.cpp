#include "Dataset.hpp"
#include <GLUT/glut.h> // GLUT, include glu.h and gl.h
#include <cassert>
#include <iostream>
#include <math.h>
#include <numeric>
#include <string>
#include <vector>

using namespace std;
// #include <windows.h> // for MS Windows

unsigned const int width = 500;
unsigned const int heigth = 500;
unsigned int M;

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  //glColor3f(1.0,1.0,1.0);
  //  glFlush();
}

void killHandler(int signum)
{
  // cout << "received signal: " << signum << endl;
  exit(signum);
}

void activeMouseMove(int x, int y)
{
  // cout << "Mouse active " << endl;
  y = width - y;
  glPointSize(4);
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_POINTS);
  glVertex2f(x, y);
  glEnd();
  glutSwapBuffers();
  // glFlush();
  // cout << x << " " << y << endl;
  // glFlush(); // Render now
}

void init()
{
  /* select clearing (background) color */
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glViewport(0, 0, heigth, width);
  glMatrixMode(GL_PROJECTION);
  glOrtho(0.0, 500.0, 0.0, 500.0, 1.0, -1.0);

  /* initialize viewing values */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  Dataset set("./dataset.txt");
  for (auto t : set.data) {
    glPointSize(4);
    glColor3f(t.A, t.B, t.C);
    glBegin(GL_POINTS);
    glVertex2f(t.x * heigth, t.y * width);
    glEnd();
    glutSwapBuffers();
  }
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv)
{
  // cout << "argc: " << argc << endl;
  // for (int i = 0; i < argc; ++i) {
  //   cout << argv[i] << endl;
  // }
  if (argc > 1) {
    M = stoi(argv[1]);
  } else {
    M = 10;
  }

  signal(SIGINT, killHandler);

  glutInit(&argc, argv); // Initialize GLUT
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(500, 500);
  /*Set the posotion of window*/
  glutInitWindowPosition(0, 0);
  glutCreateWindow("My Vindow");
  glutDisplayFunc(display);
  glutMotionFunc(activeMouseMove);
  init();
  glutMainLoop(); // Enter the event-processing loop
  return 0;
}
