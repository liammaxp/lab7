#ifndef REGISTRY_H
#define REGISTRY_H
#include <cassert>
#include <cstddef>
#include <array>
#include <utility>
#include <vector>
template <typename T>
class Registry {
private:
  std::vector<std::array<T, 1>> components;
  std::vector<bool> active;
  std::vector<size_t> freeIds;
  size_t activeCount = 0;
public:
  size_t store(const T& component){
    if (!freeIds.empty()) {
      size_t id = freeIds.back();
      components[id][0] = component;
      freeIds.pop_back();
      ++activeCount;
      active[id] = true;
      return id;
    }
    components.push_back({component});
    active.push_back(true);
    ++activeCount;
    return components.size() - 1;
  }
  size_t store(T&& component) {
    if (!freeIds.empty()) {
      size_t id = freeIds.back();
      components[id][0] = std::move(component);
      freeIds.pop_back();
      ++activeCount;
      active[id] = true;
      return id;
    }
    components.push_back({std::move(component)});
    active.push_back(true);
    ++activeCount;
    return components.size() - 1;
  }
  void remove(size_t id) {
    assert(valid(id));
    freeIds.push_back(id);
    components[id][0] = T{};
    active[id] = false;
    --activeCount;
  }

  T& operator[](size_t id) {
    assert(valid(id));
    return components[id][0];
  }
  const T& operator[](size_t id) const {
    assert(valid(id));
    return components[id][0];
  }
  bool valid(size_t id) const {
    return id < active.size() && active[id];
  }
  size_t size() const {
    return activeCount;
  }
  void reserve(size_t capacity) {
    components.reserve(capacity);
    active.reserve(capacity);
    freeIds.reserve(capacity);
  }
};

#endif  // REGISTRY_H
