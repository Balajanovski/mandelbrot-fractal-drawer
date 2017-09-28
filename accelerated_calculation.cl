// Used to index into the 1D array, so that we can use
// it effectively as a 2D array
int index(int x, int y, int width) {
  return 3*width*y + x*3;
}

// Utility integer absolute function
int iabs(int x) {
    return (x < 0) ? -x : x;
}

// Turn the raw x coordinates [0, 1] into a scaled x coordinate
// [0, 1] -> [-2, 1.25]
float mapX(float x, float x_min, float x_max) {
  return x*(iabs(x_max)) - iabs(x_min);
}

// Same purpose as mapX
// [0, 1] -> [-1.25, 1.25]
float mapY(float y, float y_min, float y_max) {
  return y*(iabs(y_max)) - iabs(y_min);
}



__kernel void render(__global char *out,  __global uchar *color, __global float *complex_plane) {

  // Set dimensions properly
    int x_dim = get_global_id(0);
    int y_dim = get_global_id(1);
    size_t width = get_global_size(0);
    size_t height = get_global_size(1);
    int idx = index(x_dim, y_dim, width);

    float x_origin = mapX((float) x_dim / width, complex_plane[0], complex_plane[1]);
    float y_origin = mapY((float) y_dim / height, complex_plane[2], complex_plane[3]);

    // The Escape time algorithm, it follows the pseudocode from Wikipedia
    // _very_ closely
    float x = 0.0;
    float y = 0.0;

    int iteration = 0;

    // This can be changed, to be more or less precise
    int max_iteration = 256;
    while(x*x + y*y <= 4 && iteration < max_iteration) {
      float xtemp = x*x - y*y + x_origin;
      y = 2*x*y + y_origin;
      x = xtemp;
      iteration++;
    }

    if(iteration == max_iteration) {
      // This coordinate did not escape, so it is in the Mandelbrot set
      out[idx] = 0;
      out[idx + 1] = 0;
      out[idx + 2] = 0;
    } else {
      // This coordinate did escape, so color based on quickly it escaped
      out[idx] = (iteration - 1) * color[0] / 255;
      out[idx + 1] = (iteration - 1) * color[1] / 255;
      out[idx + 2] = (iteration - 1) * color[2] / 255;
    }

}