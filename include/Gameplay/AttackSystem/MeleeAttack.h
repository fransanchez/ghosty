#pragma once

#include <Gameplay/AttackSystem/Attack.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class Collider;
class CollisionManager;

class MeleeAttack : public Attack
{
public:
    MeleeAttack(AttackFaction faction, float damage, float lifetime, float attackRatePerSecond, Collider* collider, CollisionManager* collisionManager);
    MeleeAttack(const MeleeAttack& other);
    MeleeAttack& operator=(const MeleeAttack& other);
    ~MeleeAttack() override;

    void attack(const sf::Vector2f& position, const sf::Vector2f& direction) override;
    void update(float deltaMilliseconds) override;
    void render(sf::RenderWindow& window) override;
    bool canAttack() override;
    Attack* clone() const override { return new MeleeAttack(*this); }

private:
    float m_lifetime;
    float m_lifetimeTimer;
    bool m_isActive;
};
