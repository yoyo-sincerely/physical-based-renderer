#ifndef RAYTRACER_IN_ONE_WEEKEND_HITABLE_LIST_HPP
#define RAYTRACER_IN_ONE_WEEKEND_HITABLE_LIST_HPP

#include "hitable.hpp"

class hitable_list : public hitable {
public:
  hitable **_list;
  int _size;

  hitable_list() {}
  hitable_list(hitable **list, int size) : _list(list), _size(size) { }
  virtual bool hit(const ray& r, float t_min, float t_max, hit_record &record) const;
};



#endif //RAYTRACER_IN_ONE_WEEKEND_HITABLE_LIST_HPP
