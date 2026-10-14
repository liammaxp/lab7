#include <iostream>
#include <cassert>
#include <memory>
#include "entity_manager.h"

int main() {
    // test here...
    //the follwing test code were provided by Copilot, and I have modified it to fit my code
    Registry<int> registry;
    size_t a = registry.store(100);
    size_t b = registry.store(200);
    size_t c = registry.store(300);
    assert(a == 0 && b == 1 && c == 2);
    assert(registry.size() == 3);
    assert(registry[b] == 200);
    registry.remove(b);
    assert(!registry.valid(b));
    assert(registry.size() == 2);
    size_t d = registry.store(400);
    assert(d == b);
    assert(registry[d] == 400);
    assert(registry.size() == 3);
    assert(!registry.valid(100));
    const Registry<int>& constRegistry = registry;
    assert(constRegistry[c] == 300);
    Registry<std::unique_ptr<int>> pointers;
    size_t pointerId = pointers.store(std::make_unique<int>(42));
    assert(*pointers[pointerId] == 42);
    pointers.remove(pointerId);
    assert(!pointers.valid(pointerId));
    std::cout << "Registry tests passed!\n";
    EntityManager<int, float, bool> entityManager;
    EntityID e0 = entityManager.createEntity();
    EntityID e1 = entityManager.createEntity();
    assert(e0 == 0 && e1 == 1);
    assert(entityManager.validEntity(e0));
    assert(!entityManager.validEntity(100));
    std::cout << "EntityManager tests passed!\n";
    int health = 100;
    entityManager.addComponent(e0, health);
    health = 200;
    entityManager.addComponent(e0, 2.5f);
    entityManager.addComponent(e1, true);
    std::cout << "EntityManager component tests passed!\n";
    assert(entityManager.hasComponent<int>(e0));
    assert(entityManager.hasComponent<float>(e0));
    assert(!entityManager.hasComponent<bool>(e0));
    assert(entityManager.getComponent<int>(e0) == 100);
    assert(entityManager.getComponent<float>(e0) == 2.5f);
    assert(entityManager.hasComponent<bool>(e1));
    assert(!entityManager.hasComponent<int>(e1));
    assert(entityManager.getComponent<bool>(e1)); 
    entityManager.getComponent<int>(e0) = 150;
    entityManager.getComponent<float>(e0) = 3.0f;
    entityManager.getComponent<bool>(e1) = false;
    assert(entityManager.getComponent<int>(e0) == 150);
    assert(!entityManager.getComponent<bool>(e1));
    std::cout << "EntityManager tests passed!\n";
    std::cout << "All ECS tests passed!\n";
    return 0;
}
