#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

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

        virtual void attack(const sf::Vector2f& position, const sf::Vector2f& direction, CollisionManager* collisionManager) = 0;
        virtual void update(float deltaTime) = 0;
        virtual void render(sf::RenderWindow& window) = 0;
        virtual bool canAttack() = 0;
        const float getRange() const {
            return m_range;
        }
        const AttackFaction getFaction() const {
            return m_faction;
        }

    protected:
        float m_range = 0.f;
        AttackFaction m_faction{ AttackFaction::Player };
};