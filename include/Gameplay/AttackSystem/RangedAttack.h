#pragma once

#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/AttackSystem/Projectile.h>
#include <Gameplay/Collisions/Collider.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <Render/Animation.h>
#include <Utils/ObjectPool.h>
#include <list>

class RangedAttack : public Attack
{
public:
    RangedAttack(AttackFaction faction,
        float damage,
        const Animation* animation,
        float projectileLifetime,
        float projectileSpeed,
        float fireRate,
        float range,
        Collider* collider);
    ~RangedAttack();

    void attack(const sf::Vector2f& position, const sf::Vector2f& direction, CollisionManager* collisionManager) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool canAttack() override;

private:

    float m_damage;
    float m_projectileLifetime;
    float m_projectileSpeed;
    float m_fireRate;   // shots per second
    float m_cooldownTimer;
    const Animation* m_animation;
    Collider* m_collider;

    ObjectPool<Projectile, 8> m_projectilesPool;
    std::list<Projectile*> m_projectiles;
};
