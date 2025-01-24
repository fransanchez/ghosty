#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class Attack
{
public:
    virtual ~Attack() = default;

    virtual void attack(const sf::Vector2f& position, const sf::Vector2f& direction, CollisionManager* collisionManager) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual bool canAttack() = 0;
};