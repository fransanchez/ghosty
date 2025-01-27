#pragma once

#include <Gameplay/AttackSystem/Attack.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class Collider;
class CollisionManager;

class MeleeAttack : public Attack
{
public:
    MeleeAttack(AttackFaction faction, float damage, float lifetime, float attackRate, Collider* collider, CollisionManager* collisionManager);
    ~MeleeAttack() override;

    void attack(const sf::Vector2f& position, const sf::Vector2f& direction) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool canAttack() override;

private:
    float m_damage;
    float m_lifetime;
    float m_lifetimeTimer;
    float m_cooldownTimer;
    float m_attackRate;
    bool m_isActive;
    sf::Vector2f m_position;
};
