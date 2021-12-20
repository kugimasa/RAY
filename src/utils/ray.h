//
// Created by kugimasa on 2021/04/10.
//
// Using the code of Ray Tracing in One Weekend
// https://raytracing.github.io/books/RayTracingInOneWeekend.html

#ifndef RAY_RAY_H
#define RAY_RAY_H

#include "vec3.h"

class ray {
 public:
  ray() {}
  ray(const vec3& a, const vec3& b, float time = 0.0f) {
    A = a;
    B = b;
    _time = time;
  }
  vec3 origin() const { return A; }
  vec3 direction() const { return B; }
  float time() const { return  _time; }
  vec3 point_at_parameter(float t) const { return A + t * B; }

  vec3 A;
  vec3 B;
  float _time;
};

#endif //RAY_RAY_H
