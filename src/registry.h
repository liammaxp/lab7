#ifndef REGISTRY_H
#define REGISTRY_H
#include <cassert>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>
template <typename T>
class Registry {
private:
  std::vector<std::optional<T>> components;
  std::vector<size_t> freeIds;
  size_t activeCount = 0;
public:
  template <typename U>
  size_t store(U&& component) {
    if (!freeIds.empty()) {
      size_t id = freeIds.back();
      components[id].emplace(std::forward<U>(component));
      freeIds.pop_back();
      ++activeCount;
      return id;
    }
    components.emplace_back(std::forward<U>(component));
    ++activeCount;
    return components.size() - 1;
  }
  void remove(size_t id) {
    assert(valid(id));
    freeIds.push_back(id);
    components[id].reset();
    --activeCount;
  }

  T& operator[](size_t id) {
    assert(valid(id));
    return *components[id];
  }
  const T& operator[](size_t id) const {
    assert(valid(id));
    return *components[id];
  }
  bool valid(size_t id) const {
    return id < components.size() && components[id].has_value();
  }
  size_t size() const {
    return activeCount;
  }
};

#endif  // REGISTRY_H
