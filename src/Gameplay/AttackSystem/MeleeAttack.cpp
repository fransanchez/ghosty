#include <Core/AudioTypes.h>
#include <Gameplay/AttackSystem/MeleeAttack.h>
#include <Gameplay/Collisions/Collider.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <cstdio>

MeleeAttack::MeleeAttack(
    AttackFaction faction,
    float damage,
    float lifetime,
    float attackRatePerSecond,
    SoundType soundType,
    Collider* collider,
    CollisionManager* collisionManager)
    :m_lifetime(lifetime),
    m_isActive(false),
    m_lifetimeTimer(0.f)
{
    m_attackRatePerSecond = 1.f / attackRatePerSecond;
    m_damage = damage;
    m_faction = faction;
    m_sound = soundType;
    m_collider = new Collider(*collider);
    m_collisionManager = collisionManager;
}

MeleeAttack::MeleeAttack(const MeleeAttack& other)
    : m_lifetime(other.m_lifetime),
    m_lifetimeTimer(other.m_lifetimeTimer),
    m_isActive(other.m_isActive)
{
    m_attackRatePerSecond = other.m_attackRatePerSecond;
    m_damage = other.m_damage;
    m_faction = other.m_faction;
    m_sound = other.m_sound;
    m_collider = new Collider(*other.m_collider);
    m_collisionManager = other.m_collisionManager;
}

MeleeAttack& MeleeAttack::operator=(const MeleeAttack& other)
{
    if (this != &other)
    {
        delete m_collider;

        m_lifetime = other.m_lifetime;
        m_lifetimeTimer = other.m_lifetimeTimer;
        m_isActive = other.m_isActive;
        m_attackRatePerSecond = other.m_attackRatePerSecond;
        m_damage = other.m_damage;
        m_faction = other.m_faction;
        m_collider = new Collider(*other.m_collider);
        m_collisionManager = other.m_collisionManager;
    }
    return *this;
}

MeleeAttack::~MeleeAttack()
{
    m_collisionManager->unregisterMeleeAttack(this);
    m_collisionManager = nullptr;
    delete m_collider;
    m_collider = nullptr;
}

void MeleeAttack::attack(const sf::Vector2f& position, const sf::Vector2f& direction)
{
    if (m_cooldownTimer <= 0.f && !m_isActive)
    {
        m_collider->setPosition(position);
        m_isActive = true;
        m_collisionManager->registerMeleeAttack(this);
        m_cooldownTimer = m_attackRatePerSecond;
        m_lifetimeTimer = m_lifetime;
    }
}

void MeleeAttack::update(float deltaMilliseconds)
{
    if (m_cooldownTimer > 0.f)
    {
        m_cooldownTimer -= deltaMilliseconds / 1000.f;
    }

    if (!m_isActive)
        return;

    m_lifetimeTimer -= deltaMilliseconds;

    if (m_lifetimeTimer <= 0.f)
    {
        m_collisionManager->unregisterMeleeAttack(this);
        m_isActive = false;
        return;
    }

}

void MeleeAttack::render(sf::RenderWindow& window)
{
#ifdef DEBUG_MODE
    if (m_isActive)
    {
        m_collider->render(window);
    }
#endif
}

bool MeleeAttack::canAttack()
{
    return !m_isActive && m_cooldownTimer <= 0.f;
}