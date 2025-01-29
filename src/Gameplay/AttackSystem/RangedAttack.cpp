#include <Gameplay/AttackSystem/Projectile.h>
#include <Gameplay/AttackSystem/RangedAttack.h>

RangedAttack::RangedAttack(
    AttackFaction faction,
    float damage,
    const Animation* animation,
    float projectileLifetime,
    float projectileSpeed,
    float fireRate,
    float range,
    Collider* collider,
    CollisionManager* collisionManager)
    : m_projectileLifetime(projectileLifetime),
    m_projectileSpeed(projectileSpeed),
    m_animation(animation)
{
    m_attackRate = (1.0 / fireRate);
    m_damage = damage;
    m_range = range;
    m_faction = faction;
    m_collider = collider;
    m_collisionManager = collisionManager;
}

RangedAttack::RangedAttack(const RangedAttack& other)
    : m_projectileLifetime(other.m_projectileLifetime),
    m_projectileSpeed(other.m_projectileSpeed),
    m_animation(other.m_animation),
    m_projectilesPool(other.m_projectilesPool)
{
    m_attackRate = other.m_attackRate;
    m_damage = other.m_damage;
    m_range = other.m_range;
    m_faction = other.m_faction;
    m_collider = new Collider(*other.m_collider);
    m_collisionManager = other.m_collisionManager;
}

RangedAttack& RangedAttack::operator=(const RangedAttack& other)
{
    if (this != &other)
    {
        delete m_collider;

        m_projectileLifetime = other.m_projectileLifetime;
        m_projectileSpeed = other.m_projectileSpeed;
        m_animation = other.m_animation;
        m_projectilesPool = other.m_projectilesPool;
        m_attackRate = other.m_attackRate;
        m_damage = other.m_damage;
        m_range = other.m_range;
        m_faction = other.m_faction;
        m_collider = new Collider(*other.m_collider);
        m_collisionManager = other.m_collisionManager;
    }
    return *this;
}
RangedAttack::~RangedAttack()
{
    m_collisionManager = nullptr;
    delete m_collider;
    m_collider = nullptr;
    // Return all active projectiles to the pool
    for (auto& projectile : m_projectiles)
    {
        m_projectilesPool.release(*projectile);
    }
    m_projectiles.clear();
}

void RangedAttack::attack(const sf::Vector2f& position, const sf::Vector2f& direction)
{
    if (m_cooldownTimer <= 0.0f)
    {
        Projectile& projectile = m_projectilesPool.get();
        Projectile::ProjectileDescriptor descriptor;
        descriptor.position = position;
        descriptor.direction = direction;
        descriptor.projectileSpeed = m_projectileSpeed;
        descriptor.projectileLife = m_projectileLifetime;
        descriptor.damage = m_damage;

        Collider* projectileCollider = new Collider(*m_collider);
        projectile.init(descriptor, m_animation, projectileCollider, m_collisionManager);
        m_projectiles.push_front(&projectile);
        m_cooldownTimer = m_attackRate;
        m_collisionManager->registerProjectile(&projectile, m_faction);
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

        if (projectile->isExpired() || projectile->isMarkedForDestruction())
        {
            m_collisionManager->unregisterProjectile(projectile, m_faction);
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