#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics.hpp>
#include <Render/Animation.h>
#include <cstdio>

class Projectile : public GameObject
{
    public:
        Projectile() = default;

        ~Projectile() override;

        void init(const sf::Vector2f& pos, const sf::Vector2f& dir, float projectile_speed, float life, const Animation& anim);

        sf::Vector2f getPosition() const;
        const Animation& getAnimation() const;

        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;

        bool isExpired() const;

    private:
        sf::Sprite m_sprite;
        sf::Vector2f m_direction;
        float m_lifetime{ 0.0f };
        float m_projectileSpeed{ 0.0f };
        Animation m_animation;
};
