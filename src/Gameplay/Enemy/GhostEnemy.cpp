#include <Gameplay/CollisionManager.h>
#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/Enemy/GhostEnemy.h>
#include <cmath>

void GhostEnemy::update(float deltaMilliseconds)
{
    m_stateTimer += deltaMilliseconds;

    handleState(deltaMilliseconds);

    float deltaSeconds = deltaMilliseconds / 1000.f;

    if (m_currentState == EnemyState::Patrol)
    {
        PatrolAreaCollision patrolCollision = m_collisionManager->checkPatrolArea(m_collider);

        if (!patrolCollision.inside)
        {
            printf("Warning: GhostEnemy is outside its patrol area.\n");
            return; // Prevent movement if outside patrol area
        }

        // Reverse direction if touching edges
        if (patrolCollision.leftEdge)
        {
            m_movingRight = true;
        }
        else if (patrolCollision.rightEdge)
        {
            m_movingRight = false;
        }
        m_direction.x = m_movingRight ? 1.0f : -1.0f;
    }

    // Move based on direction and speed
    m_position.x += m_direction.x * m_speed.x * deltaSeconds;

    // Update animation frames
    if (m_currentAnimation)
    {
        m_currentAnimation->update(deltaSeconds);
        m_sprite.setTexture(*m_currentAnimation->getCurrentFrame());
    }
    if (m_movingRight) {
        m_sprite.setScale(1.0f, 1.0f);
    }
    else {
        m_sprite.setScale(-1.0f, 1.0f);
    }
    m_sprite.setPosition(m_position);
    // Sync collider position
    m_collider->setPosition(m_position);
}

void GhostEnemy::handleState(float deltaMilliseconds)
{
    switch (m_currentState)
    {
    case EnemyState::Idle:
    {
        // Ghost hovers in place
        m_direction = { 0.f, 0.f };

        // Transition to Patrol after IDLE_DURATION
        if (m_stateTimer >= IDLE_DURATION)
        {
            changeState(EnemyState::Patrol);
            m_stateTimer = 0.0f; // Reset timer for the new state
            m_direction = { 1.0f, 0.0f };
        }
        break;
    }

    case EnemyState::Patrol:
    {
        if (m_stateTimer >= PATROL_DURATION)
        {
            m_stateTimer = 0.0f;
            changeState(EnemyState::Idle);
            m_direction = { 0.0f, 0.0f }; // Stop moving
        }
        break;
    }

    case EnemyState::Chase:
        // Add chase logic
        break;
    case EnemyState::Attack:
    {
        // Ghost performs an attack
        if (!m_attacks.empty() && m_attacks[m_currentAttackIndex]->canAttack())
        {
            sf::Vector2f attackDirection = m_direction;
            m_attacks[m_currentAttackIndex]->attack(m_position, attackDirection, m_collisionManager);
        }

        // After attacking, return to Chase
        m_currentState = EnemyState::Chase;
        break;
    }

    default:
        break;
    }
}

void GhostEnemy::handleCollisions()
{
    // To-Do;
}