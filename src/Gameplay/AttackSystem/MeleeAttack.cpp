#include <Gameplay/AttackSystem/MeleeAttack.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <cstdio>

MeleeAttack::MeleeAttack(float damage, float lifetime, float attackRate, Collider* collider)
    : m_damage(damage),
    m_lifetime(lifetime),
    m_attackRate(attackRate),
    m_collider(collider),
    m_collisionManager(nullptr),
    m_isActive(false),
    m_cooldownTimer(0.f),
    m_lifetimeTimer(0.f)
{
}

MeleeAttack::~MeleeAttack()
{
    delete m_collider;
    m_collider = nullptr;
}

void MeleeAttack::attack(const sf::Vector2f& position, const sf::Vector2f& direction, CollisionManager* collisionManager)
{
    if (m_cooldownTimer <= 0.f && !m_isActive)
    {
        m_position = position;
        m_collisionManager = collisionManager;
        m_collider->setPosition(m_position);
        m_isActive = true;
        m_cooldownTimer = m_attackRate;
        m_lifetimeTimer = m_lifetime;
    }
}

void MeleeAttack::update(float deltaTime)
{
    if (m_cooldownTimer > 0.f)
    {
        m_cooldownTimer -= deltaTime;
    }

    if (!m_isActive)
        return;

    m_lifetimeTimer -= deltaTime;
    if (m_lifetimeTimer <= 0.f)
    {
        m_isActive = false;
        return;
    }

    // Check for collisions
    auto collisions = m_collisionManager->checkCollisionsWith(m_collider);
    for (auto* target : collisions)
    {
        // Apply damage or handle collision logic
        printf("MeleeAttack: Collision detected with target!\n");
        // Apply damage logic here (if needed)
    }
}

void MeleeAttack::render(sf::RenderWindow& window)
{
    if (m_isActive)
    {
        m_collider->render(window);
    }
}

bool MeleeAttack::canAttack()
{
    return !m_isActive && m_cooldownTimer <= 0.f;
}
