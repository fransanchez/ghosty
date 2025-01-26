#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/CollisionManager.h>
#include <Gameplay/Enemy/GhostEnemy.h>
#include <cmath>

void GhostEnemy::update(float deltaMilliseconds)
{
    // Update timer state and call parent to update normally
    m_stateTimer += deltaMilliseconds;

    Enemy::update(deltaMilliseconds);
}

void GhostEnemy::handleCollisions()
{
    // To-Do
}

void GhostEnemy::handleIdleState()
{
    m_direction = { 0.f, 0.f };

    if (m_stateTimer >= IDLE_DURATION)
    {
        changeState(EnemyState::Patrol);
        m_stateTimer = 0.0f;
        m_direction = { 1.0f, 0.0f };
    }
}

void GhostEnemy::handlePatrolState()
{
    if (m_stateTimer >= PATROL_DURATION)
    {
        m_stateTimer = 0.0f;
        changeState(EnemyState::Idle);
        m_direction = { 0.0f, 0.0f }; // Stop moving
    }
    else 
    {
        // Continue patrolling
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
}

void GhostEnemy::handleChaseState()
{
    sf::Vector2f playerPosition = m_collisionManager->getPlayerPosition();
    PatrolAreaCollision patrolCollision = m_collisionManager->checkPatrolArea(m_collider);
    bool canSeePlayer = m_collisionManager->isPlayerInsideArea(m_enemySight.getGlobalBounds());
    sf::FloatRect patrolArea = patrolCollision.areaBounds;
    bool isPlayerInsidePatrolArea = m_collisionManager->isPlayerInsideArea(patrolArea);

    // If we already caught the player, return or attack
    if (m_collisionManager->isPlayerInsideArea(m_collider->getBounds()))
    {
        m_direction = { 0.0f, 0.0f };

        if (!m_attacks.empty() && m_attacks[m_currentAttackIndex]->canAttack())
        {
            changeState(EnemyState::Attack);
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
        if (!isPlayerInsidePatrolArea)
        {
            if ((m_direction.x < 0 && patrolCollision.leftEdge) ||
                (m_direction.x > 0 && patrolCollision.rightEdge))
            {
                m_direction.x = 0.0f;
            }
        }
    }
    else
    {
        // If the player is in the patrol area but not visible, move toward them
        if (isPlayerInsidePatrolArea)
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
            m_direction = { 0.0f, 0.0f };
        }
    }
}

void GhostEnemy::handleAttackState() 
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
