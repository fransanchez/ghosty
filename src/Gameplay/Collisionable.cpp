#include <Gameplay/Collisionable.h>

Collisionable::~Collisionable() {
    m_collider.reset();
    m_collisionManager = nullptr;
}

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
