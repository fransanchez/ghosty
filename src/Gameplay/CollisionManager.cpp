#include <Gameplay/CollisionManager.h>
#include <Gameplay/Collider.h>
#include <SFML/Graphics/Shape.hpp>

CollisionManager::~CollisionManager()
{
    m_colliders.clear();
    m_groundShapes.clear();
    m_wallShapes.clear();
}

void CollisionManager::registerCollider(Collider* collider)
{
    m_colliders.push_back(collider);
}

void CollisionManager::unregisterCollider(Collider* collider)
{
    m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), collider), m_colliders.end());
}

void CollisionManager::setGroundShapes(const std::vector<sf::Shape*>& groundShapes)
{
    m_groundShapes = groundShapes;
}

void CollisionManager::setWallShapes(const std::vector<sf::Shape*>& wallShapes)
{
    m_wallShapes = wallShapes;
}

bool CollisionManager::checkIsGrounded(const Collider* collider) const
{
    sf::FloatRect colliderBounds = collider->getBounds();

    for (const auto* groundShape : m_groundShapes)
    {
        sf::FloatRect groundBounds = groundShape->getGlobalBounds();

        sf::FloatRect colliderBottom(colliderBounds.left, colliderBounds.top + colliderBounds.height - 1.f, colliderBounds.width, GROUND_COLLISION_MARGIN);
        sf::FloatRect groundTop(groundBounds.left, groundBounds.top, groundBounds.width, GROUND_COLLISION_MARGIN);

        if (colliderBottom.intersects(groundTop))
        {
            return true;
        }
    }

    return false;
}

WallCollision CollisionManager::checkWalls(const Collider* collider) const
{
    WallCollision result;
    sf::FloatRect colliderBounds = collider->getBounds();

    for (const auto* wallShape : m_wallShapes)
    {
        sf::FloatRect wallBounds = wallShape->getGlobalBounds();

        if (colliderBounds.intersects(wallBounds))
        {
            result.collided = true;

            // Right collision
            if (colliderBounds.left < wallBounds.left &&
                colliderBounds.left + colliderBounds.width > wallBounds.left)
            {
                result.rightCollision = true;
            }

            // Left collision
            if (colliderBounds.left > wallBounds.left &&
                colliderBounds.left < wallBounds.left + wallBounds.width)
            {
                result.leftCollision = true;
            }

            // Top collision
            if (colliderBounds.top < wallBounds.top + wallBounds.height &&
                colliderBounds.top + colliderBounds.height > wallBounds.top
                && colliderBounds.top + colliderBounds.height >= wallBounds.top + wallBounds.height)
            {
                result.topCollision = true;
            }
        }
    }

    return result;
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
