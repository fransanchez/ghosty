#pragma once

#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/Collisions/HitCollider.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class CollisionManager;

class MeleeAttack : public Attack
{
public:
    MeleeAttack(int damage, float lifetime, float attackRate, HitCollider* collider);
    ~MeleeAttack() override;

    void attack(const sf::Vector2f& position, const sf::Vector2f& direction, CollisionManager* collisionManager) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool canAttack() override;

private:
    int m_damage;
    float m_lifetime;
    float m_lifetimeTimer;
    float m_cooldownTimer;
    float m_attackRate;
    HitCollider* m_collider;
    CollisionManager* m_collisionManager;
    bool m_isActive;
    sf::Vector2f m_position;
};
