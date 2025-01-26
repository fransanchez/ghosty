#include <Gameplay/CollisionManager.h>
#include <Gameplay/Collider.h>
#include <SFML/Graphics/Shape.hpp>

CollisionManager::~CollisionManager()
{
    m_colliders.clear();
    m_groundShapes.clear();
    m_wallShapes.clear();
    m_enemyPatrolAreasShapes.clear();
    m_playerCollider = nullptr;
}

void CollisionManager::registerCollider(Collider* collider)
{
    if (std::find(m_colliders.begin(), m_colliders.end(), collider) == m_colliders.end()) {
        m_colliders.push_back(collider);
    }
}

void CollisionManager::unregisterCollider(Collider* collider)
{
    auto it = std::find(m_colliders.begin(), m_colliders.end(), collider);
    if (it != m_colliders.end()) {
        m_colliders.erase(it);
    }
}

void CollisionManager::registerPlayer(Collider* playerCollider)
{
    m_playerCollider = playerCollider;
}

void CollisionManager::unregisterPlayer()
{
    m_playerCollider = nullptr;
}

void CollisionManager::setGroundShapes(const std::vector<sf::Shape*>& groundShapes)
{
    m_groundShapes = groundShapes;
}

void CollisionManager::setWallShapes(const std::vector<sf::Shape*>& wallShapes)
{
    m_wallShapes = wallShapes;
}

void CollisionManager::setEnemyPatrolAreasShapes(const std::vector<sf::Shape*>& patrolAreasShapes)
{
    m_enemyPatrolAreasShapes = patrolAreasShapes;
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

PatrolAreaCollision CollisionManager::checkPatrolArea(const Collider* collider, const sf::Shape* patrolArea) const
{
    PatrolAreaCollision result;
    sf::FloatRect colliderBounds = collider->getBounds();
    sf::FloatRect patrolBounds = patrolArea->getGlobalBounds();

    result.areaBounds = patrolBounds;

    if (colliderBounds.intersects(patrolBounds))
    {
        result.inside = true;

        if (colliderBounds.left <= patrolBounds.left)
        {
            result.leftEdge = true;
        }
        if (colliderBounds.left + colliderBounds.width >= patrolBounds.left + patrolBounds.width)
        {
            result.rightEdge = true;
        }
    }
    return result;
}

sf::Vector2f CollisionManager::getPlayerPosition() const
{
    if (!m_playerCollider)
    {
        printf("Error: Player is not registered in the CollisionManager.\n");
        return { 0.f, 0.f };
    }

    sf::FloatRect playerBounds = m_playerCollider->getBounds();
    return { playerBounds.left + playerBounds.width / 2.f, playerBounds.top + playerBounds.height / 2.f };
}

bool CollisionManager::isPlayerInsideArea(const sf::FloatRect& area) const
{
    if (!m_playerCollider)
    {
        printf("Warning: Player collider not registered.\n");
        return false;
    }

    sf::FloatRect playerBounds = m_playerCollider->getBounds();
    return area.intersects(playerBounds);
}

const sf::Shape* CollisionManager::getClosestPatrolArea(const sf::Vector2f& spawnPoint) const
{
    const sf::Shape* closestArea = nullptr;
    float minArea = std::numeric_limits<float>::max();

    for (const auto* patrolShape : m_enemyPatrolAreasShapes)
    {
        sf::FloatRect patrolBounds = patrolShape->getGlobalBounds();

        if (patrolBounds.contains(spawnPoint))
        {
            float area = patrolBounds.width * patrolBounds.height;
            if (area < minArea)
            {
                minArea = area;
                closestArea = patrolShape;
            }
        }
    }

    return closestArea;
}