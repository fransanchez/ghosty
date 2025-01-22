#pragma once

#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/AttackSystem/Projectile.h>
#include <Render/Animation.h>
#include <Utils/ObjectPool.h>
#include <list>

class RangedAttack : public Attack
{
public:
    RangedAttack(float damage, const Animation* animation, float projectileLifetime, float projectileSpeed, float fireRate);
    ~RangedAttack();

    void attack(const sf::Vector2f& position, const sf::Vector2f& direction) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool canAttack() override;

private:

    float m_damage;
    float m_projectileLifetime;
    float m_projectileSpeed;
    float m_fireRate;   // shots per second
    float m_cooldownTimer;

    ObjectPool<Projectile, 8> m_projectilesPool;
    std::list<Projectile*> m_projectiles;
    const Animation* m_animation;
};
