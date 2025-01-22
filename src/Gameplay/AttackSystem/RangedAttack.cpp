#include <Gameplay/AttackSystem/RangedAttack.h>

RangedAttack::RangedAttack(
    float damage,
    const Animation& animation,
    float projectileLifetime,
    float projectileSpeed,
    float fireRate)
    : m_damage(damage),
    m_projectileLifetime(projectileLifetime),
    m_projectileSpeed(projectileSpeed),
    m_fireRate(1.0f / fireRate),
    m_cooldownTimer(0.0f),
    m_animation(animation)
{
}

void RangedAttack::attack(const sf::Vector2f& position, const sf::Vector2f& direction)
{
    if (m_cooldownTimer <= 0.0f)
    {
        m_projectiles.emplace_back(position, direction, m_projectileLifetime, m_animation);
        m_cooldownTimer = m_fireRate;
    }
}

void RangedAttack::update(float deltaTime)
{
    if (m_cooldownTimer > 0.0f)
    {
        m_cooldownTimer -= deltaTime;
    }

    for (auto it = m_projectiles.begin(); it != m_projectiles.end();)
    {
        it->lifetime -= deltaTime;

        if (it->lifetime <= 0.0f)
        {
            it = m_projectiles.erase(it);
        }
        else
        {
            it->position += it->direction * m_projectileSpeed * deltaTime;
            it->animation.update(deltaTime);
            ++it;
        }
    }
}

void RangedAttack::render(sf::RenderWindow& window)
{
    for (const auto& projectile : m_projectiles)
    {
        projectile.animation.render(window, projectile.position);
    }
}

bool RangedAttack::canAttack() {
    return m_cooldownTimer <= 0.0f;
}
