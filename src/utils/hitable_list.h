//
// Created by kugimasa on 2021/04/10.
//
// Using the code of Ray Tracing in One Weekend
// https://raytracing.github.io/books/RayTracingInOneWeekend.html

#ifndef RAY_UTILS_HITABLE_LIST_H_
#define RAY_UTILS_HITABLE_LIST_H_
#include "hitable.h"

class hitable_list : public hitable {
 public:
  hitable_list() {}
  hitable_list(hitable **l, int n) {
    list = l;
    list_size = n;
  }
  virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
  hitable **list;
  int list_size;
};

bool hitable_list::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
  hit_record temp_rec;
  bool hit_anything = false;
  double closest_so_far = t_max;
  for (int i = 0; i < list_size; i++) {
    if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }
  return hit_anything;
}
#endif //RAY_UTILS_HITABLE_LIST_H_
