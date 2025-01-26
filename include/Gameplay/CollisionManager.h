#pragma once

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

class Collider;
namespace sf
{
    class Shape;
}

struct WallCollision
{
    bool collided = false;
    bool leftCollision = false;
    bool rightCollision = false;
    bool topCollision = false;
};

struct PatrolAreaCollision
{
    bool inside = false;
    bool leftEdge = false;
    bool rightEdge = false;
    sf::FloatRect areaBounds;
};

class CollisionManager
{
public:
    ~CollisionManager();

    void registerCollider(Collider* collider);
    void unregisterCollider(Collider* collider);
    void registerPlayer(Collider* playerCollider);
    void unregisterPlayer();

    void setGroundShapes(const std::vector<sf::Shape*>& groundShapes);
    void setWallShapes(const std::vector<sf::Shape*>& wallShapes);
    void setEnemyPatrolAreasShapes(const std::vector<sf::Shape*>& patrolAreasShapes);

    bool checkIsGrounded(const Collider* collider) const;
    WallCollision checkWalls(const Collider* collider) const;
    PatrolAreaCollision checkPatrolArea(const Collider* collider, const sf::Shape* patrolArea) const;
    bool isPlayerInsideArea(const sf::FloatRect& area) const;

    const sf::Shape* getClosestPatrolArea(const sf::Vector2f& spawnPoint) const;

    std::vector<Collider*> checkCollisionsWith(const Collider* collider) const;
    sf::Vector2f getPlayerPosition() const;

private:
    const float GROUND_COLLISION_MARGIN = 6.f;

    std::vector<Collider*> m_colliders;
    std::vector<sf::Shape*> m_groundShapes;
    std::vector<sf::Shape*> m_wallShapes;
    std::vector<sf::Shape*> m_enemyPatrolAreasShapes;

    Collider* m_playerCollider = nullptr; // New
};
