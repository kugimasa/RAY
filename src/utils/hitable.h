//
// Created by kugimasa on 2021/04/10.
//
// Using the code of Ray Tracing in One Weekend
// https://raytracing.github.io/books/RayTracingInOneWeekend.html

#ifndef RAY_UTILS_HITABLE_H_
#define RAY_UTILS_HITABLE_H_
#include "ray.h"

class material;

struct hit_record {
  float t;
  vec3 p;
  vec3 normal;
  material *mat_ptr;
};

class hitable {
 public:
  virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};

#endif //RAY_UTILS_HITABLE_H_
