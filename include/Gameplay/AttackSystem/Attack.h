#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class Collider;
class CollisionManager;

enum class AttackFaction
{
    Player,
    Enemy
};

class Attack
{

    public:
        virtual ~Attack() = default;

        virtual Attack* clone() const = 0;
        virtual void attack(const sf::Vector2f& position, const sf::Vector2f& direction) = 0;
        virtual void update(float deltaMilliseconds) = 0;
        virtual void render(sf::RenderWindow& window) = 0;
        virtual bool canAttack() = 0;
        const float getRange() const {
            return m_range;
        }
        const AttackFaction getFaction() const {
            return m_faction;
        }
        const Collider* getCollider() const {
            return m_collider;
        }
        const int getDamage() const {
            return m_damage;
        }

    protected:
        int m_damage = 0;
        float m_range = 0.f;
        float m_cooldownTimer = 0.f;
        float m_attackRatePerSecond = 0.f;   // attacks per second

        AttackFaction m_faction{ AttackFaction::Player };
        Collider* m_collider{ nullptr };
        CollisionManager* m_collisionManager{ nullptr };
};