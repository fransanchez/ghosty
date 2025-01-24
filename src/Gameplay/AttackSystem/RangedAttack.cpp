#include <Gameplay/AttackSystem/Projectile.h>
#include <Gameplay/AttackSystem/RangedAttack.h>

RangedAttack::RangedAttack(
    float damage,
    const Animation* animation,
    float projectileLifetime,
    float projectileSpeed,
    float fireRate,
    Collider* collider)
    : m_damage(damage),
    m_projectileLifetime(projectileLifetime),
    m_projectileSpeed(projectileSpeed),
    m_fireRate(1.0f / fireRate),
    m_cooldownTimer(0.0f),
    m_animation(animation),
    m_collider(collider)
{
}

RangedAttack::~RangedAttack()
{
    delete m_collider;
    m_collider = nullptr;
    // Return all active projectiles to the pool
    for (auto& projectile : m_projectiles)
    {
        m_projectilesPool.release(*projectile);
    }
    m_projectiles.clear(); // Clear the active list
}

void RangedAttack::attack(const sf::Vector2f& position, const sf::Vector2f& direction, CollisionManager* collisionManager)
{
    if (m_cooldownTimer <= 0.0f)
    {
        Projectile& projectile = m_projectilesPool.get();
        Projectile::ProjectileDescriptor descriptor;
        descriptor.position = position;
        descriptor.direction = direction;
        descriptor.projectileSpeed = m_projectileSpeed;
        descriptor.projectileLife = m_projectileLifetime;

        Collider* projectileCollider = new Collider(*m_collider);
        projectile.init(descriptor, m_animation, projectileCollider, collisionManager);
        m_projectiles.push_front(&projectile);
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
        Projectile* projectile = *it;
        projectile->update(deltaTime);

        if (projectile->isExpired())
        {
            m_projectilesPool.release(*projectile);
            it = m_projectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void RangedAttack::render(sf::RenderWindow& window)
{
    for (const auto& projectile : m_projectiles)
    {
        projectile->render(window);
    }
}

bool RangedAttack::canAttack() {
    return m_cooldownTimer <= 0.0f;
}
