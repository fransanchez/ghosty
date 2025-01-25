#pragma once

#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/Collider.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class CollisionManager;

class MeleeAttack : public Attack
{
public:
    MeleeAttack(float damage, float lifetime, Collider* collider);
    ~MeleeAttack() override;

    void attack(const sf::Vector2f& position, const sf::Vector2f& direction, CollisionManager* collisionManager) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool canAttack() override;

private:
    float m_damage;
    float m_lifetime;
    float m_cooldownTimer;
    Collider* m_collider;
    CollisionManager* m_collisionManager;
    bool m_isActive;
    sf::Vector2f m_position;
};
