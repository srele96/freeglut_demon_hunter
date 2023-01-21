#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutCreateWindow("GLEW Test");
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cerr << "Error " << glewGetErrorString(err);
  }
  std::cout << "Status: Using GLEW" << glewGetString(GLEW_VERSION) << "\n";
  return 0;
}
