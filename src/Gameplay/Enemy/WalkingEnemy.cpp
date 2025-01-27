#include <cassert>
#include <cmath>
#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <Gameplay/Enemy/WalkingEnemy.h>
#include <Utils/Constants.h>

void WalkingEnemy::update(float deltaMilliseconds)
{
    // Update timer state and call parent to update normally
    m_stateTimer += deltaMilliseconds;

    Enemy::update(deltaMilliseconds);
}

void WalkingEnemy::handleIdleState()
{
    if (isPlayerInArea() && isPlayerInSight()) {
        changeState(EnemyState::TargetLocked);
        return;
    }

    if (m_stateTimer >= WALKING_ENEMY_IDLE_DURATION)
    {
        changeState(EnemyState::Patrol);
        m_stateTimer = 0.0f;
        m_direction.x = m_movingRight ? 1.0f : -1.0f;
    }
}

void WalkingEnemy::handlePatrolState()
{
    if (isPlayerInArea() && isPlayerInSight()) {
        changeState(EnemyState::TargetLocked);
        return;
    }

    if (m_stateTimer >= WALKING_ENEMY_PATROL_DURATION)
    {
        m_stateTimer = 0.0f;
        changeState(EnemyState::Idle);
    }
    else 
    {
        moveWithinAreaEdges();
    }
}

void WalkingEnemy::handleChaseState()
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
                changeState(EnemyState::Idle);
            }
        }
    }
    else
    {
        // If the player is in the patrol area but not visible, move toward them
        if (playerReachable)
        {
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
        }
        else
        {
            // If the player is not in sight and outside the patrol area, return to idle
            changeState(EnemyState::Idle);
        }
    }
}

void WalkingEnemy::handleAttackState()
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

void WalkingEnemy::handleTargetLockedState()
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
void WalkingEnemy::handleReturnToOriginState()
{
    // We should never enter this state, but if for any reason we do, throw an exception
    assert(false && "ReturnToOriginState should be unreachable for walking enemy");
}

