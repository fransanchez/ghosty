#include <cmath>
#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <Gameplay/Enemy/FlyingEnemy.h>
#include <Utils/Constants.h>

void FlyingEnemy::update(float deltaMilliseconds)
{
    m_oscillationTimer += deltaMilliseconds / 1000.f;

    Enemy::update(deltaMilliseconds);
}

void FlyingEnemy::handleIdleState()
{
    if (isPlayerInArea() && isPlayerInSight()) {
        changeState(EnemyState::TargetLocked);
        return;
    }

    if (m_position.y - m_originalPosition.y <= ENEMY_RETURN_POSITION_TOLERANCE) {
        // Switch to patrol immediately if we are at the right height
        changeState(EnemyState::Patrol);
        m_direction = { 1.0f, 0.0f };
    }
    else {
        // otherwise return
        changeState(EnemyState::ReturnToOrigin);
    }
}

void FlyingEnemy::handlePatrolState()
{
    if (isPlayerInArea() && isPlayerInSight()) {
        changeState(EnemyState::TargetLocked);
        return;
    }

    moveWithinAreaEdges();

    applyOscillation();
}

void FlyingEnemy::handleChaseState()
{
    sf::Vector2f playerPosition = m_collisionManager->getPlayerPosition();
    bool canSeePlayer = m_collisionManager->isPlayerInsideArea(m_enemySight.getGlobalBounds());

    bool playerReachable = isPlayerInArea();

    // If we already caught the player, return or attack
    if (playerReachable && isPlayerInRange())
    {
        if (!m_attacks.empty() && m_attacks[m_currentAttackIndex]->canAttack())
        {
            changeState(EnemyState::Attack);
        }
        else {
            changeState(EnemyState::TargetLocked);
        }
        return;
    }

    if (canSeePlayer && playerReachable)
    {
        // If we can see the player, move towards them
        if (playerPosition.x < m_position.x)
        {
            m_direction.x = -1.0f;
            m_movingRight = false;
        }
        else
        {
            m_direction.x = 1.0f;
            m_movingRight = true;
        }

        if (playerPosition.y < m_position.y) {
            m_direction.y = -1.0f;
        }
        else {
            m_direction.y = 1.0f;
        }
    }
    else
    {
        changeState(EnemyState::ReturnToOrigin);
    }
}

void FlyingEnemy::handleAttackState()
{
    if (!m_attacks.empty() && m_attacks[m_currentAttackIndex]->canAttack())
    {
        sf::Vector2f playerPosition = m_collisionManager->getPlayerPosition();
        sf::Vector2f attackDirection = playerPosition - m_position;

        // Normalize direction
        float magnitude = std::sqrt(attackDirection.x * attackDirection.x + attackDirection.y * attackDirection.y);
        if (magnitude != 0.0f) {
            attackDirection.x /= magnitude;
            attackDirection.y /= magnitude;
        }

        m_attacks[m_currentAttackIndex]->attack(m_position, attackDirection);
        SoundType sound = m_attacks[m_currentAttackIndex]->getSoundType();
        AudioManager::getInstance()->playSoundEffect(sound);
    }

    if (m_currentAnimation && m_currentAnimation->isFinished())
    {
        changeState(EnemyState::Idle);
    }
}

void FlyingEnemy::handleTargetLockedState()
{
    if (isPlayerInRange())
    {
        if (!m_attacks.empty() && m_attacks[m_currentAttackIndex]->canAttack())
        {
            changeState(EnemyState::Attack);
        }
    }
    else
    {
        changeState(EnemyState::Chase);
    }
}

void FlyingEnemy::handleReturnToOriginState()
{
    // We want to return to origin height, but same x we are now
    float distanceToOriginY = m_originalPosition.y - m_position.y;

    if (std::abs(distanceToOriginY) > ENEMY_RETURN_POSITION_TOLERANCE) {
        if (m_position.y < m_originalPosition.y) {
            m_direction.y = 1.f;
        }
        else {
            m_direction.y = -1.f;
        }
        moveWithinAreaEdges();
    }
    else {
        m_direction.y = 0.f;
        m_oscillationTimer = 0.f;
        changeState(EnemyState::Patrol);
    }
}

void FlyingEnemy::applyOscillation()
{
    float oscillationOffset = FLYING_ENEMY_OSCILLATION_RANGE * std::sin(m_oscillationTimer * FLYING_ENEMY_OSCILLATION_SPEED);
    m_position.y = m_position.y + oscillationOffset;
    if (oscillationOffset <= 0) {
        m_direction.y = -1.f;
    }
    else {
        m_direction.y = 1.f;
    }
}