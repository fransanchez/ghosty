#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <Gameplay/Enemy/Enemy.h>
#include <SFML/Graphics/RenderWindow.hpp>

Enemy::~Enemy()
{
    // Enemy doesn't own the patrol areas, that's the level
    m_patrolArea = nullptr;
}

bool Enemy::init(const EnemyDescriptor& enemyDescriptor, CollisionManager* collisionManager)
{
    if (!initEntity(enemyDescriptor.animations, enemyDescriptor.attacks, enemyDescriptor.maxLife, enemyDescriptor.position))
    {
        return false;
    }

    m_collider = enemyDescriptor.collider;
    m_collisionManager = collisionManager;
    m_patrolArea = enemyDescriptor.patrolArea;
    m_patrolSpeed = enemyDescriptor.speed;
    m_chaseSpeed = { m_patrolSpeed.x * 1.5f, m_patrolSpeed.y * 1.5f };
    float startingX = isMovingRight() ? 1.0f : -1.0f;
    sf::Vector2f startingDirection = { startingX, 0.f };
    setDirection(startingDirection);
    setPosition(enemyDescriptor.position);
    setSpeed({ 0.0f, 0.0f });
    m_type = enemyDescriptor.type;
    m_originalPosition = enemyDescriptor.position;
    m_sightArea = { enemyDescriptor.sightArea.x, enemyDescriptor.sightArea.y };
    m_enemySight.setSize({ m_sightArea.x, m_sightArea.y });
    m_enemySight.setOrigin(m_enemySight.getOrigin().x, m_enemySight.getSize().y / 2.f);

    return true;
}

void Enemy::update(float deltaMilliseconds)
{
    // First always update ememy position before calling parent
    // to update sprites and other items
    if (!isDead()) {

        if (shouldEndInvincibility()) {
            setInvincibility(false);
            changeState(EnemyState::Idle);
        }

        updateSight();

        handleCollisions();

        handleState(deltaMilliseconds);

        float deltaSeconds = deltaMilliseconds / 1000.f;
        m_position.x += m_direction.x * m_speed.x * deltaSeconds;
        m_position.y += m_direction.y * m_speed.y * deltaSeconds;
    }

    Entity::update(deltaMilliseconds);
}


void Enemy::render(sf::RenderWindow& window)
{
    Entity::render(window);

#ifdef DEBUG_MODE
    m_enemySight.setFillColor(sf::Color(0, 0, 255, 50));
    m_enemySight.setOutlineColor(sf::Color::Blue);
    m_enemySight.setOutlineThickness(1.0f);
    window.draw(m_enemySight);
#endif
}

void Enemy::handleCollisions()
{
    if (!isInvincible()) {
        int damage = m_collisionManager->checkEnemyHurtingCollisions(m_collider);

        if (damage > 0)
        {
            reduceLives(damage);
            if (getCurrentLives() == 0)
            {
                setIsDead(true);
                m_attacks[m_currentAttackIndex]->stopAttack();
                changeState(EnemyState::Dead);
            }
            else
            {
                setInvincibility(true);
                changeState(EnemyState::Hurt);
            }
            AudioManager::getInstance()->playSoundEffect(getHurtSoundType());
        }
    }
}

void Enemy::handleState(float deltaMilliseconds)
{
    switch (m_currentState)
    {
    case EnemyState::Idle:
        handleIdleState();
        break;
    case EnemyState::Patrol:
        handlePatrolState();
        break;
    case EnemyState::Chase:
        handleChaseState();
        break;
    case EnemyState::TargetLocked:
        handleTargetLockedState();
        break;
    case EnemyState::ReturnToOrigin:
        handleReturnToOriginState();
        break;
    case EnemyState::Attack:
        handleAttackState();
        break;
    case EnemyState::Hurt:
    case EnemyState::Dead:
    default:
        break;
    }
}

void Enemy::changeState(EnemyState newState)
{
    if (m_currentState != newState)
    {
        m_currentState = newState;
        setSpeedForState();
        updateAnimationType();
    }
}

void Enemy::updateAnimationType()
{
    AnimationType animationType = AnimationType::Idle;

    switch (m_currentState)
    {
    case EnemyState::Idle:
        animationType = AnimationType::Idle;
        break;
    case EnemyState::Patrol:
        animationType = AnimationType::Walk;
        break;
    case EnemyState::Chase:
        animationType = AnimationType::Walk;
        break;
    case EnemyState::TargetLocked:
        animationType = AnimationType::Idle;
        break;
    case EnemyState::ReturnToOrigin:
        animationType = AnimationType::Walk;
        break;
    case EnemyState::Attack:
        animationType = AnimationType::Attack;
        break;
    case EnemyState::Hurt:
        animationType = AnimationType::Hurt;
        break;
    case EnemyState::Dead:
        animationType = AnimationType::Death;
        break;
    }

    setAnimationType(animationType);
}

void Enemy::setSpeedForState()
{
    switch (m_currentState)
    {
    case EnemyState::Patrol:
        setSpeed(m_patrolSpeed);
        break;
    case EnemyState::Chase:
        setSpeed(m_chaseSpeed);
        break;
    case EnemyState::ReturnToOrigin:
        setSpeed(m_patrolSpeed);
        break;
    default:
        setSpeed({ 0.0f, 0.0f });
        break;
    }
}

void Enemy::updateSight()
{
    m_enemySight.setPosition(m_position);

    if (isMovingRight()) // Facing right
    {
        m_enemySight.setSize({ m_sightArea.x, m_sightArea.y });
    }
    else // Facing left
    {
        m_enemySight.setSize({ -m_sightArea.x, m_sightArea.y });
    }
}

void Enemy::moveWithinAreaEdges() {

    PatrolAreaCollision patrolCollision = m_collisionManager->checkPatrolArea(m_collider, m_patrolArea);
    if (!patrolCollision.inside)
    {
        printf("Warning: Enemy is outside its patrol area.\n");
        return;
    }

    // Reverse direction if touching edges
    if (patrolCollision.leftEdge)
    {
        setMovingRight(true);
    }
    else if (patrolCollision.rightEdge)
    {
        setMovingRight(false);
    }
    m_direction.x = isMovingRight() ? 1.0f : -1.0f;
}

bool Enemy::isPlayerInSight() {
    return m_collisionManager->isPlayerInsideArea(m_enemySight.getGlobalBounds());
}

bool Enemy::isPlayerInArea() {
    return m_collisionManager->isPlayerInsideArea(m_patrolArea->getGlobalBounds());
}
void Enemy::reset()
{
    m_currentState = EnemyState::Idle;
    setPosition(m_originalPosition);
    Entity::reset();
}
