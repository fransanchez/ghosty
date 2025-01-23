#pragma once

#include <vector>
#include <SFML/Graphics/Rect.hpp>

class Collider;

class CollisionManager
{
public:
    void registerCollider(Collider* collider);
    void unregisterCollider(Collider* collider);

    bool checkGround(const Collider* collider) const;

    bool checkWalls(const Collider* collider) const;

    std::vector<Collider*> checkCollisionsWith(const Collider* collider) const;

private:
    std::vector<Collider*> m_colliders;
};
