#include <GL/glew.h>
#include <SDL2/SDL.h>

GLuint program;
GLint attribute_coord2d;
GLuint vbo_triangle;

#include <cstdlib>
#include <iostream>

char* file_read(const char* filename) {
  SDL_RWops* rw = SDL_RWFromFile(filename, "rb");
  if (rw == NULL) return NULL;

  Sint64 res_size = SDL_RWsize(rw);
  char* res = (char*)malloc(res_size + 1);

  Sint64 nb_read_total = 0;
  Sint64 nb_read = 1;
  char* buf = res;
  while (nb_read_total < res_size && nb_read != 0) {
    nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
    nb_read_total += nb_read;
    buf += nb_read;
  }
  SDL_RWclose(rw);
  if (nb_read_total != res_size) {
    free(res);
    return NULL;
  }

  res[nb_read_total] = '\0';
  return res;
}

void print_log(GLuint object) {
  GLint log_length = 0;
  if (glIsShader(object)) {
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
  } else if (glIsProgram(object)) {
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
  } else {
    std::cerr << "print_log: Not a shader or program\n";
    return;
  }

  char* log = (char*)malloc(log_length);

  if (glIsShader(object)) {
    glGetShaderInfoLog(object, log_length, NULL, log);
  } else if (glIsProgram(object)) {
    glGetProgramInfoLog(object, log_length, NULL, log);
  }

  std::cerr << log << "\n";
  free(log);
}

GLuint create_shader(const char* filename, GLenum type) {
  const GLchar* source = file_read(filename);
  if (source == NULL) {
    std::cerr << "Error opening " << filename << ": " << SDL_GetError() << "\n";
    return 0;
  }

  GLuint res = glCreateShader(type);
  const GLchar* sources[]{
#ifdef GL_ES_VERSION_2_0
      "#version 100\n"  // OpenGL ES 2.0
#else
      "#version 120\n"  // OpenGL 2.1
#endif
      ,
      source};

  glShaderSource(res, 2, sources, NULL);
  free((void*)source);

  glCompileShader(res);
  GLint compile_ok = GL_FALSE;
  glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok == GL_FALSE) {
    std::cerr << filename << ":";
    print_log(res);
    glDeleteShader(res);
    return 0;
  }

  return res;
}

bool init_resources() {
  GLfloat triangle_vertices[]{0.0, 0.8, -0.8, -0.8, 0.8, -0.8};
  glGenBuffers(1, &vbo_triangle);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices,
               GL_STATIC_DRAW);

  GLint compile_ok = GL_FALSE;
  GLint link_ok = GL_FALSE;

  // Calculate pixels on the screen
  GLuint vs = create_shader("triangle.v.glsl", GL_VERTEX_SHADER);
  if (vs == 0) return false;

  // Color pixels on the screen
  GLuint fs = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER);
  if (fs == 0) return false;

  // Link program with shaders
  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    std::cerr << "Error in glLinkProgram\n";
    print_log(program);
    return false;
  }

  const char* attribute_name = "coord2d";
  attribute_coord2d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord2d == -1) {
    std::cerr << "Could not bind attribute\n";
    return false;
  }

  return true;
}

void render(SDL_Window* window) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);

  glEnableVertexAttribArray(attribute_coord2d);

  glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glDisableVertexAttribArray(attribute_coord2d);

  SDL_GL_SwapWindow(window);
}

void main_loop(SDL_Window* window) {
  while (true) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT) {
        return;
      }
    }
    render(window);
  }
}

void free_resources() {
  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo_triangle);
}

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow(
      "My first triangle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640,
      480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

  if (window == NULL) {
    std::cerr << "Error: Ca't create window\n";
    return EXIT_FAILURE;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 1);
  if (SDL_GL_CreateContext(window) == NULL) {
    std::cerr << "Error: SDL_GL_CreateContext " << SDL_GetError() << "\n";
    return EXIT_FAILURE;
  }

  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK) {
    std::cerr << "Error: glewInit " << glewGetErrorString(glew_status)
              << std::endl;
    return EXIT_FAILURE;
  }

  if (!GLEW_VERSION_2_0) {
    std::cerr << "Error. Your graphics card doesn't support OpenGL 2.0\n";
    return EXIT_FAILURE;
  }

  if (!init_resources()) {
    return EXIT_FAILURE;
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  main_loop(window);

  free_resources();

  return EXIT_SUCCESS;
}
