#pragma once

#include <SFML/Graphics.hpp>
#include <Render/Animation.h>
#include <cstdio>

class Projectile
{
public:
    Projectile() = default;

    void init(const sf::Vector2f& pos, const sf::Vector2f& dir, float projectile_speed, float life, const Animation& anim);

    ~Projectile();

    sf::Vector2f getPosition() const;
    const Animation& getAnimation() const;

    void update(float deltaTime);

    bool isExpired() const;

private:
    sf::Vector2f m_position;
    sf::Vector2f m_direction;
    float m_lifetime{ 0.0f };
    float m_projectileSpeed{ 0.0f };
    Animation m_animation;
};
