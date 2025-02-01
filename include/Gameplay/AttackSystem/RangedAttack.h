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
        float fireRateSeconds,
        float range,
        SoundType soundType,
        Collider* collider,
        CollisionManager* collisionManager);
    RangedAttack(const RangedAttack& other);
    RangedAttack& operator=(const RangedAttack& other);
    ~RangedAttack() override;

    void attack(const sf::Vector2f& position, const sf::Vector2f& direction) override;
    void update(float deltaMilliseconds) override;
    void render(sf::RenderWindow& window) override;
    bool canAttack() override;
    void stopAttack() override;

    Attack* clone() const override { return new RangedAttack(*this); }

private:
    float m_projectileLifetime;
    float m_projectileSpeed;
    const Animation* m_animation;

    ObjectPool<Projectile, 8> m_projectilesPool;
    std::list<Projectile*> m_projectiles;
};
