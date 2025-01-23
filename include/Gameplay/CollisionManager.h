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

class CollisionManager
{
public:
    ~CollisionManager();

    void registerCollider(Collider* collider);
    void unregisterCollider(Collider* collider);

    void setGroundShapes(const std::vector<sf::Shape*>& groundShapes);
    void setWallShapes(const std::vector<sf::Shape*>& wallShapes);

    bool checkIsGrounded(const Collider* collider) const;
    WallCollision checkWalls(const Collider* collider) const;

    std::vector<Collider*> checkCollisionsWith(const Collider* collider) const;

private:
    const float GROUND_COLLISION_MARGIN = 6.f;

    std::vector<Collider*> m_colliders;
    std::vector<sf::Shape*> m_groundShapes;
    std::vector<sf::Shape*> m_wallShapes;
};
