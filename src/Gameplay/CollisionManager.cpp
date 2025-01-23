#include <Gameplay/CollisionManager.h>
#include <Gameplay/Collider.h>

void CollisionManager::registerCollider(Collider* collider)
{
    m_colliders.push_back(collider);
}

void CollisionManager::unregisterCollider(Collider* collider)
{
    m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), collider), m_colliders.end());
}

bool CollisionManager::checkGround(const Collider* collider) const
{
    for (const auto* other : m_colliders)
    {
        if (other == collider)
            continue;

        if (collider->getBounds().intersects(other->getBounds()))
        {
            return true;
        }
    }
    return false;
}

bool CollisionManager::checkWalls(const Collider* collider) const
{
    for (const auto* other : m_colliders)
    {
        if (other == collider)
            continue;

        if (collider->getBounds().intersects(other->getBounds()))
        {
            return true;
        }
    }
    return false;
}

std::vector<Collider*> CollisionManager::checkCollisionsWith(const Collider* collider) const
{
    std::vector<Collider*> collisions;
    for (const auto* other : m_colliders)
    {
        if (other == collider)
            continue;

        if (collider->getBounds().intersects(other->getBounds()))
        {
            collisions.push_back(const_cast<Collider*>(other));
        }
    }
    return collisions;
}
