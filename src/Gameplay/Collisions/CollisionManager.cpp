#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/AttackSystem/Projectile.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <Gameplay/Collisions/Collider.h>
#include <Gameplay/Enemy/Enemy.h>
#include <Gameplay/Player/Player.h>
#include <SFML/Graphics/Shape.hpp>

CollisionManager::~CollisionManager()
{
    m_groundShapes.clear();
    m_wallShapes.clear();
    m_enemyPatrolAreasShapes.clear();

    for (Projectile* proj : m_enemyProjectiles) {
        proj = nullptr;
    }
    m_enemyProjectiles.clear();

    for (Projectile* proj : m_playerProjectiles) {
        proj = nullptr;
    }
    m_playerProjectiles.clear();

    for (Enemy* enemy : m_enemies) {
        enemy = nullptr;
    }
    m_enemies.clear();
    m_player = nullptr;
}

void CollisionManager::registerEnemy(Enemy* enemy)
{
    m_enemies.push_back(enemy);
}

void CollisionManager::unregisterEnemy(Enemy* enemy)
{
    auto it = std::find(m_enemies.begin(), m_enemies.end(), enemy);
    if (it != m_enemies.end()) {
        m_enemies.erase(it);
    }
}

void CollisionManager::registerPlayer(Player* player)
{
    m_player = player;
}

void CollisionManager::unregisterPlayer()
{
    m_player = nullptr;
}

void CollisionManager::registerProjectile(Projectile* projectile, AttackFaction faction)
{
    if (faction == AttackFaction::Player) {
        m_playerProjectiles.push_back(projectile);
    }
    else {
        m_enemyProjectiles.push_back(projectile);
    }
}
void CollisionManager::unregisterProjectile(Projectile* projectile, AttackFaction faction)
{
    std::vector<Projectile*> projectilesVector = 
        (faction == AttackFaction::Player) ? m_playerProjectiles : m_enemyProjectiles;

    auto it = std::find(projectilesVector.begin(), projectilesVector.end(), projectile);
    if (it != projectilesVector.end()) {
        projectilesVector.erase(it);
    }
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
    if (!m_player)
    {
        printf("Error: Player is not registered in the CollisionManager.\n");
        return { 0.f, 0.f };
    }

    sf::FloatRect playerBounds = m_player->getCollider()->getBounds();
    return { playerBounds.left + playerBounds.width / 2.f, playerBounds.top + playerBounds.height / 2.f };
}

bool CollisionManager::isPlayerInsideArea(const sf::FloatRect& area) const
{
    if (!m_player)
    {
        printf("Warning: Player collider not registered.\n");
        return false;
    }

    sf::FloatRect playerBounds = m_player->getCollider()->getBounds();
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