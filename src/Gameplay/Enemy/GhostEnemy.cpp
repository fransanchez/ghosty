#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/CollisionManager.h>
#include <Gameplay/Enemy/GhostEnemy.h>
#include <cmath>


constexpr float OSCILLATION_RANGE = 0.7f;
constexpr float OSCILLATION_SPEED = 2.f;
constexpr float RETURN_TOLERANCE = 2.0f;

void GhostEnemy::update(float deltaMilliseconds)
{
    m_oscillationTimer += deltaMilliseconds / 1000.f;

    Enemy::update(deltaMilliseconds);
}

void GhostEnemy::handleIdleState()
{
    if (isPlayerInArea() && isPlayerInSight()) {
        changeState(EnemyState::TargetLocked);
        return;
    }

    if (m_position.y - m_originalPosition.y <= RETURN_TOLERANCE) {
        // Switch to patrol immediately if we are at the right height
        changeState(EnemyState::Patrol);
        m_direction = { 1.0f, 0.0f };
    }
    else {
        // otherwise return
        changeState(EnemyState::ReturnToOrigin);
    }
}

void GhostEnemy::handlePatrolState()
{
    if (isPlayerInArea() && isPlayerInSight()) {
        changeState(EnemyState::TargetLocked);
        return;
    }

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

    applyOscillation();
}

void GhostEnemy::handleChaseState()
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

void GhostEnemy::handleTargetLockedState()
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

void GhostEnemy::handleReturnToOriginState()
{
    // We want to return to origin height, but same x we are now
    float distanceToOriginY = m_originalPosition.y - m_position.y;

    if (std::abs(distanceToOriginY) > RETURN_TOLERANCE) {
        if (m_position.y < m_originalPosition.y) {
            m_direction.y = 1.f;
        }
        else {
            m_direction.y = -1.f;
        }
    }
    else {
        m_direction.y = 0.f;
        m_oscillationTimer = 0.f;
        changeState(EnemyState::Patrol);
    }
}

void GhostEnemy::applyOscillation()
{
    float oscillationOffset = OSCILLATION_RANGE * std::sin(m_oscillationTimer * OSCILLATION_SPEED);
    m_position.y = m_position.y + oscillationOffset;
    if (oscillationOffset <= 0) {
        m_direction.y = -1.f;
    }
    else {
        m_direction.y = 1.f;
    }
}

bool GhostEnemy::isPlayerInRange() {
    return m_collisionManager->isPlayerInsideArea(m_collider->getBounds());
}
