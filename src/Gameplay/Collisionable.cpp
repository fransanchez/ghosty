#include <Gameplay/Collisionable.h>

void Collisionable::setCollider(std::unique_ptr<Collider> collider)
{
    m_collider = std::move(collider);
}

Collider* Collisionable::getCollider() const
{
    return m_collider.get();
}

void Collisionable::setCollisionManager(CollisionManager* manager)
{
    m_collisionManager = manager;
}

CollisionManager* Collisionable::getCollisionManager() const
{
    return m_collisionManager;
}
