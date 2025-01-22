#pragma once

#include <Gameplay/AttackSystem/Attack.h>
#include <Render/Animation.h>
#include <vector>

class RangedAttack : public Attack
{
public:
    RangedAttack(float damage, const Animation& animation, float projectileLifetime, float projectileSpeed, float fireRate);

    void attack(const sf::Vector2f& position, const sf::Vector2f& direction) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool canAttack() override;

private:
    struct Projectile
    {
        sf::Vector2f position;
        sf::Vector2f direction;
        float lifetime;
        Animation animation;

        Projectile(const sf::Vector2f& pos, const sf::Vector2f& dir, float life, const Animation& anim)
            : position(pos), direction(dir), lifetime(life), animation(anim) {
        }
    };

    float m_damage;
    float m_projectileLifetime;
    float m_projectileSpeed;
    float m_fireRate;   // shots per second
    float m_cooldownTimer;

    std::vector<Projectile> m_projectiles;
    Animation m_animation;
};
