//
// Created by kugimasa on 2021/04/10.
//
// Using the code of Ray Tracing in One Weekend
// https://raytracing.github.io/books/RayTracingInOneWeekend.html

#ifndef RAY_OBJECTS_SPHERE_H_
#define RAY_OBJECTS_SPHERE_H_

#include "../utils/hitable.h"

class sphere : public hitable {
 public:
  sphere();
  sphere(vec3 cen, float r) : center(cen), radius(r) {};
  sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};
  virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
  vec3 center;
  float radius;
  material *mat_ptr;
};

bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
  vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - radius * radius;
  float discriminant = b * b - a * c;
  if (discriminant > 0) {
    float temp = (-b - sqrt(b * b - a * c)) / a;
    if (t_min < temp && temp < t_max) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      rec.mat_ptr = mat_ptr;
      return true;
    }
    ///another intersecting point
    temp = (-b + sqrt(b * b - a * c)) / a;
    if (t_min < temp && temp < t_max) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center) / radius;
      rec.mat_ptr = mat_ptr;
      return true;
    }
  }
  return false;
}

/// 移動球
class moving_sphere : public hitable {
 public:
  moving_sphere() {}
  moving_sphere(vec3 cen0, vec3 cen1,
                float t0, float t1,
                float r,
                material *m) :
                center0(cen0), center1(cen1),
                time0(t0), time1(t1),
                radius(r),
                mat_ptr(m) {};
  virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
  vec3 center(float time) const;
  vec3 center0, center1;
  float time0, time1;
  float radius;
  material *mat_ptr;
};

/// time0 から time1の間で移動する球の
/// 時刻 t における中心座標
vec3 moving_sphere::center(float time) const {
  return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

/// 交差判定
bool moving_sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
  vec3 oc = r.origin() - center(r.time());
  float a = dot(r.direction(), r.direction());
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - radius * radius;
  float discriminant = b * b - a * c;
  if (discriminant > 0) {
    float temp = (-b - sqrt(discriminant)) / a;
    if (t_min < temp && temp < t_max) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center(r.time())) / radius;
      rec.mat_ptr = mat_ptr;
      return true;
    }
    /// 別の交差点
    temp = (-b + sqrt(discriminant)) / a;
    if (t_min < temp && temp < t_max) {
      rec.t = temp;
      rec.p = r.point_at_parameter(rec.t);
      rec.normal = (rec.p - center(r.time())) / radius;
      rec.mat_ptr = mat_ptr;
      return true;
    }
  }
  return false;
}
#endif //RAY_OBJECTS_SPHERE_H_
