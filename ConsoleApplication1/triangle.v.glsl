attribute vec2 coord2d;

void main(void) {
  gl_Position[0] = coord2d.x;
  gl_Position[1] = 0;
  gl_Position[2] = 0.0;
  gl_Position[3] = 1.0;
}
