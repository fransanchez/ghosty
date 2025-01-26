#include <cassert>
#include <cmath>
#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/CollisionManager.h>
#include <Gameplay/Enemy/SkeletonEnemy.h>

void SkeletonEnemy::update(float deltaMilliseconds)
{
    // Update timer state and call parent to update normally
    m_stateTimer += deltaMilliseconds;

    Enemy::update(deltaMilliseconds);
}

void SkeletonEnemy::handleIdleState()
{
    if (isPlayerInArea() && isPlayerInSight()) {
        changeState(EnemyState::TargetLocked);
        return;
    }

    if (m_stateTimer >= IDLE_DURATION)
    {
        changeState(EnemyState::Patrol);
        m_stateTimer = 0.0f;
        m_direction = { 1.0f, 0.0f };
    }
}

void SkeletonEnemy::handlePatrolState()
{
    if (isPlayerInArea() && isPlayerInSight()) {
        changeState(EnemyState::TargetLocked);
        return;
    }

    if (m_stateTimer >= PATROL_DURATION)
    {
        m_stateTimer = 0.0f;
        changeState(EnemyState::Idle);
    }
    else 
    {
        // Continue patrolling
        PatrolAreaCollision patrolCollision = m_collisionManager->checkPatrolArea(m_collider, m_patrolArea);
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
}

void SkeletonEnemy::handleChaseState()
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
                //m_speed.x = 0.0f;
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

void SkeletonEnemy::handleAttackState()
{
    if (!m_attacks.empty() && m_attacks[m_currentAttackIndex]->canAttack())
    {
        sf::Vector2f attackDirection = m_direction;
        m_attacks[m_currentAttackIndex]->attack(m_position, attackDirection, m_collisionManager);
    }

    if (m_currentAnimation && m_currentAnimation->isFinished())
    {
        changeState(EnemyState::Idle);
    }
}

void SkeletonEnemy::handleTargetLockedState()
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
void SkeletonEnemy::handleReturnToOriginState()
{
    // We should never enter this state, but if for any reason we do, throw an exception
    assert(false && "ReturnToOriginState should be unreachable for Skeleton enemy");
}

bool SkeletonEnemy::isPlayerInRange() {
    return m_collisionManager->isPlayerInsideArea(m_collider->getBounds());
    // To-Do, add range of weapon for ranged attacks.
}
