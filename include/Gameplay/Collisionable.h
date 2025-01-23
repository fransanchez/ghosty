#pragma once

#include <Gameplay/Collider.h>
#include <Gameplay/GameObject.h>
#include <memory>

class CollisionManager;

class Collisionable : public GameObject
{
public:
    Collisionable() = default;
    virtual ~Collisionable() = default;

    void setCollider(std::unique_ptr<Collider> collider);
    Collider* getCollider() const;

    void setCollisionManager(CollisionManager* manager);
    CollisionManager* getCollisionManager() const;

    virtual void handleCollisions() = 0;
protected:
    std::unique_ptr<Collider> m_collider;
    CollisionManager* m_collisionManager = nullptr;
};
