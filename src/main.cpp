//
// Created by kugimasa on 2021/04/10.
//

#include <iostream>
#include "objects/sphere.h"
#include "utils/hitable_list.h"
#include "utils/output_file.h"
#include "utils/colors.h"
#include "utils/my_print.h"
#include "camera/camera.h"
#include "material/material.h"

vec3 color(const ray &r, hitable *world, int depth) {
  hit_record rec;
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    ray scattered;
    vec3 attenuation;
    /// 再起処理
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation * color(scattered, world, depth + 1);
    } else {
      return BLACK;
    }
  }
  /// 背景色(グラデーション)の描画
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

void drawPix(unsigned char *data,
             unsigned int w, unsigned int h,
             unsigned int x, unsigned int y,
             unsigned int r, unsigned int g, unsigned int b)
{
  unsigned char *p;
  p = data + (h - y) * w * 3 + x * 3;
  p[0] = (unsigned char)r;
  p[1] = (unsigned char)g;
  p[2] = (unsigned char)b;
}

void render(unsigned char *data, unsigned int nx, unsigned int ny, int ns)
{
  /// シーンデータ
  /// オブジェクトデータ
  hitable *list[2];
  list[0] = new moving_sphere(vec3(-0.5, 0, -1),vec3(0.5, 0, -1),
                              0.0, 1.0, 0.5,
                              new lambertian(KUGI_COLOR));
  list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(GREY));
  hitable *world = new hitable_list(list, 2);
  /// カメラ設定
  vec3 lookfrom(0.0, 1.0, 5.0);
  vec3 lookat(0.0, 0.0,-10.0);
  float vfov {49.0f};
  float dist_to_focus {10.0f};
  float aperture {0.0f};
  float aspect = float(nx)/float(ny);
  float t0 {0.0f}, t1 {1.0f};
  camera cam(lookfrom, lookat, Y_UP, vfov, aspect, aperture, dist_to_focus, t0, t1);
  float progress = 0.0;
  int img_size = nx * ny;
  std::cout << "========== Render ==========" << std::endl;
  for (int j = 0; j < ny; j++) {
    for (int i = 0; i < nx; i++) {
      vec3 col(0, 0, 0);
      for (int s = 0; s < ns; s++) {
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);
        ray r = cam.get_ray(u, v);
        vec3 p = r.point_at_parameter(2.0);
        col += color(r, world, 0);
      }
      col /= float(ns);
      col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);
      progress = float (i + j * nx) / img_size;
      flush_progress(progress);
      drawPix(data, nx, ny, i, j, ir, ig, ib);
    }
  }
  std::cout << "\n========== Finish ==========" << std::endl;
}

int main() {
  int nx = 800;
  int ny = 600;
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

  /// 背景色の指定
  memset(output.data, 0xFF, output.width * output.height * output.ch);
  /// 描画処理
  render(output.data, nx, ny, ns);

  if (pngFileEncodeWrite(&output, "output.png")) {
    freeBitmapData(&output);
    return -1;
  }
  freeBitmapData(&output);
  return 0;
}