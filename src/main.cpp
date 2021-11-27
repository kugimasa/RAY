//
// Created by kugimasa on 2021/04/10.
//

#include <iostream>
#include <fstream>
#include "objects/sphere.h"
#include "utils/hitable_list.h"
#include "utils/output_file.h"
#include "utils/my_print.h"
#include "camera/camera.h"
#include "material/material.h"

vec3 color(const ray &r, hitable *world) {
  hit_record rec;
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    return 0.5 * color(ray(rec.p, target - rec.p), world);
  }
    ///Drawing blue sky
  else {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
  }
}

void flush_progress(float progress) {
  int bar_width = 20;
  std::cout << "\r [";
  int pos = bar_width * progress;
  for (int i = 0; i < bar_width; ++i) {
    if (i < pos) std::cout <<  "=";
    else if (i == pos) std::cout <<  ">";
    else std::cout <<  " ";
  }
  std::cout << "] " << int(progress * 100.0) << " %" << std::flush;
}

void render(int nx, int ny, int ns)
{
  std::ofstream outputfile("Diffuse_3sphere.ppm");
  outputfile << "P3\n" << nx << " " << ny << " " << "255\n";
  ///list of sphere objects
  hitable *list[4];
  list[0] = new sphere(vec3(0, 0, -1), 0.5);
  list[1] = new sphere(vec3(-1, 0, -1), 0.5);
  list[2] = new sphere(vec3(1, 0, -1), 0.5);
  list[3] = new sphere(vec3(0, -100.5, -1), 100);
  hitable *world = new hitable_list(list, 4);
  camera cam;
  float progress = 0.0;
  int img_size = nx * ny;
  std::cout << "========== Render ==========" << std::endl;
  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      vec3 col(0, 0, 0);
      for (int s = 0; s < ns; s++) {
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);
        ray r = cam.get_ray(u, v);
        vec3 p = r.point_at_parameter(2.0);
        col += color(r, world);
      }
      col /= float(ns);
      col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);
      progress = float (i + (ny - j - 1) * nx) / img_size;
      flush_progress(progress);
      outputfile << ir << " " << ig << " " << ib << "\n";
    }
  }
  std::cout << "\n========== Finish ==========" << std::endl;
}

int main() {
  int nx = 400;
  int ny = 400;
  int ns = 100;

  /// BitMap
  BITMAPDATA_t output;
  output.width = nx;
  output.height = ny;
  output.ch = 3;
  /// Malloc
  output.data = (unsigned char*)malloc(sizeof(unsigned char) * output.width * output.height * output.ch);
  if (output.data == NULL) {
    error_print("Memory Allocation Error");
    return -1;
  }

  /// Render
  memset(output.data, 0xFF, output.width * output.height * output.ch);

  // render(nx, ny, ns);


  if (pngFileEncodeWrite(&output, "output.png")) {
    freeBitmapData(&output);
    return -1;
  }
  freeBitmapData(&output);
  return 0;
}