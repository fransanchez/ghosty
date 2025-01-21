#pragma once
#include <Gameplay/Attack.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <Render/Animation.h>

class ProjectileAttack : public Attack
{
public:
    ProjectileAttack(const Animation& animation, float speed, float lifetime);

    void activate(const sf::Vector2f& position, const sf::Vector2f& direction) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool isFinished() const override;

private:
    sf::Sprite m_sprite;
    sf::Vector2f m_direction;
    float m_speed;
    float m_lifetime;
    float m_elapsedTime{ 0.0f };
    bool m_finished{ false };
    Animation m_animation;
};
