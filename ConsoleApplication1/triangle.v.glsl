attribute vec2 coord2d;

void main(void) {
  gl_Position = vec4(coord2d.x + .1, coord2d.y - .1, 0.0, 1.0);
}
