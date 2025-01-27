#pragma once

#include <Gameplay/Collisions/Collider.h>

class HitCollider : public Collider
{
public:
    HitCollider(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Vector2f& centerOffset, int damage)
        : Collider(position, size, centerOffset), m_damage(damage) {
    }

    HitCollider(const Collider& baseCollider, int damage)
        : Collider(baseCollider.getCenter(), baseCollider.getBounds().getSize(), baseCollider.getCenterOffset()), m_damage(damage) {
    }

    int getDamage() const { return m_damage; }

private:
    int m_damage;
};
