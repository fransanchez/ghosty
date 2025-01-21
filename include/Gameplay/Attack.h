#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <Render/Animation.h>

class Attack
{
public:
    virtual ~Attack() = default;

    virtual void activate(const sf::Vector2f& position, const sf::Vector2f& direction) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual bool isFinished() const = 0;
};