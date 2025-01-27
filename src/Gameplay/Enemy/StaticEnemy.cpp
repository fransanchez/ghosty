#include <cassert>
#include <cmath>
#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <Gameplay/Enemy/StaticEnemy.h>
#include <Utils/Constants.h>

void StaticEnemy::update(float deltaMilliseconds)
{
    // Update timer state and call parent to update normally
    m_watchTimer += deltaMilliseconds;
    Enemy::update(deltaMilliseconds);
}

void StaticEnemy::handleIdleState()
{
    if (isPlayerInArea() && isPlayerInSight()) {
        changeState(EnemyState::TargetLocked);
    }
    else if (std::abs(m_position.x - m_originalPosition.x) > ENEMY_RETURN_POSITION_TOLERANCE) {
        changeState(EnemyState::ReturnToOrigin);
    }
    else {
        if (m_watchTimer >= STATIC_ENEMY_WATCH_DURATION)
        {
            m_watchTimer = 0.0f;
            m_movingRight = !m_movingRight;
            m_direction.x = m_movingRight ? 1.0f : -1.0f;
        }
    }
}

void StaticEnemy::handlePatrolState()
{
    assert(false && "Patrol State should be unreachable for static enemy");
}

void StaticEnemy::handleChaseState()
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

    if (canSeePlayer)
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

        // Stop at patrol area edges
        if (!playerReachable)
        {
            PatrolAreaCollision patrolCollision = m_collisionManager->checkPatrolArea(m_collider, m_patrolArea);
            if ((m_direction.x < 0 && patrolCollision.leftEdge) ||
                (m_direction.x > 0 && patrolCollision.rightEdge))
            {
                changeState(EnemyState::ReturnToOrigin);
            }
        }
    }
    else
    {
        changeState(EnemyState::ReturnToOrigin);
    }
}

void StaticEnemy::handleAttackState()
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
        m_attacks[m_currentAttackIndex]->attack(m_position, attackDirection, m_collisionManager);
    }

    if (m_currentAnimation && m_currentAnimation->isFinished())
    {
        changeState(EnemyState::Idle);
    }
}

void StaticEnemy::handleTargetLockedState()
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
void StaticEnemy::handleReturnToOriginState()
{
    // We want to return to origin X, but same Y we are now
    float distanceToOriginx = m_originalPosition.x - m_position.x;

    if (std::abs(distanceToOriginx) > ENEMY_RETURN_POSITION_TOLERANCE) {
        if (m_position.x < m_originalPosition.x) {
            m_direction.x = 1.f;
            m_movingRight = true;
        }
        else {
            m_direction.x = -1.f;
            m_movingRight = false;
        }
    }
    else {
        m_direction.x = (-m_direction.x);
        m_movingRight = !m_movingRight;
        changeState(EnemyState::Idle);
    }
}

