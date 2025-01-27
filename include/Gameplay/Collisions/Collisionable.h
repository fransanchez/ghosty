#pragma once

#include <Gameplay/Collisions/Collider.h>
#include <Gameplay/GameObject.h>
#include <memory>

class CollisionManager;

class Collisionable : public GameObject
{
public:
    Collisionable() = default;
    Collisionable(Collider* collider, CollisionManager* manager);
    virtual ~Collisionable();

    Collider* getCollider() const;

    CollisionManager* getCollisionManager() const;

    virtual void handleCollisions() = 0;
protected:
    Collider* m_collider;
    CollisionManager* m_collisionManager = nullptr;
};
