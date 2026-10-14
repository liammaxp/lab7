#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <cstddef>  // size_t
#include <array>
#include <cassert>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include "pack.h"  // Pack from prep
#include "registry.h"
using EntityID = size_t;

template <typename... ComponentTypes>
class EntityManager {
private:
  static constexpr size_t INVALID_INDEX = std::numeric_limits<size_t>::max();
  std::tuple<Registry<ComponentTypes>...> registries;
  std::vector<std::array<size_t, sizeof...(ComponentTypes)>> componentIndices;
  std::vector<bool> activeEntities;
  std::vector<EntityID> freeIds;
public:
  EntityID createEntity() {
    if (!freeIds.empty()) {
      EntityID id = freeIds.back();
      freeIds.pop_back();
      activeEntities[id] = true;
      componentIndices[id].fill(INVALID_INDEX);
      return id;
    }
    EntityID id = activeEntities.size();
    std::array<size_t, sizeof...(ComponentTypes)> indices;
    indices.fill(INVALID_INDEX);
    componentIndices.push_back(indices);
    activeEntities.push_back(true);
    return id;
  }
  bool validEntity(EntityID id) const {
    return id < activeEntities.size() && activeEntities[id];
  }
  template <typename ComponentType>
  bool hasComponent(EntityID id) const {
    static_assert(Pack<ComponentTypes...>::template contains<ComponentType>,"Invalid component type");
    assert(validEntity(id));
    constexpr int typeIndex = Pack<ComponentTypes...>::template index<ComponentType>;
    return componentIndices[id][typeIndex] != INVALID_INDEX;
  }
  template <typename ComponentType>
  ComponentType& getComponent(EntityID id) {
    static_assert(Pack<ComponentTypes...>::template contains<ComponentType>,"Invalid component type");
    assert(hasComponent<ComponentType>(id));
    constexpr int typeIndex = Pack<ComponentTypes...>::template index<ComponentType>;
    size_t index = componentIndices[id][typeIndex];
    return std::get<typeIndex>(registries)[index];
  }

  template <typename ComponentType>
  void addComponent(EntityID id, ComponentType&& component) {
    using T = std::remove_cvref_t<ComponentType>;
    static_assert(Pack<ComponentTypes...>::template contains<T>,"Invalid component type");
    assert(validEntity(id));
    assert(!hasComponent<T>(id));
    constexpr int typeIndex = Pack<ComponentTypes...>::template index<T>;
    size_t index = std::get<typeIndex>(registries).store(std::forward<ComponentType>(component));
    componentIndices[id][typeIndex] = index;
  }
};

#endif  // ENTITY_MANAGER_H
