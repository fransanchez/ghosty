#include <Gameplay/AttackSystem/MeleeAttack.h>
#include <Gameplay/CollisionManager.h>
#include <cstdio>

MeleeAttack::MeleeAttack(float damage, float lifetime, Collider* collider)
    : m_damage(damage),
    m_lifetime(lifetime),
    m_cooldownTimer(0.f),
    m_collider(collider),
    m_collisionManager(nullptr),
    m_isActive(false)
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
        m_cooldownTimer = m_lifetime;

        printf("MeleeAttack: Attack triggered at position (%f, %f)\n", position.x, position.y);
    }
}

void MeleeAttack::update(float deltaTime)
{
    if (!m_isActive)
        return;

    m_cooldownTimer -= deltaTime;

    if (m_cooldownTimer <= 0.f)
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
    return !m_isActive;
}
