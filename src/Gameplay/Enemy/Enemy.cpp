#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/CollisionManager.h>
#include <Gameplay/Enemy/Enemy.h>
#include <SFML/Graphics/RenderWindow.hpp>

Enemy::~Enemy()
{
    for (auto attack : m_attacks)
        delete attack;

    for (auto& [type, animation] : *m_animations)
        delete animation;

    delete m_animations;

    // Enemy doesn't own the patrol areas, that's the level
    m_patrolArea = nullptr;
}

bool Enemy::init(const EnemyDescriptor& enemyDescriptor,
    Collider* collider,
    CollisionManager* collisionManager)
{
    m_animations = enemyDescriptor.animations;
    m_attacks = enemyDescriptor.attacks;
    m_collider = collider;
    m_collisionManager = collisionManager;

    m_patrolArea = enemyDescriptor.patrolArea;

    m_patrolSpeed = enemyDescriptor.speed;
    m_chaseSpeed = { m_patrolSpeed.x * 1.5f, m_patrolSpeed.y * 1.5f };
    m_speed = m_patrolSpeed;

    m_sightArea = { enemyDescriptor.sightArea.x, enemyDescriptor.sightArea.y };

    if (m_animations->count(AnimationType::Idle))
    {
        m_currentAnimation = (*m_animations)[AnimationType::Idle];
        m_sprite.setTexture(*m_currentAnimation->getCurrentFrame());
    }
    else
    {
        printf("Error: Idle animation is missing\n");
        return false;
    }

    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);
    setPosition(enemyDescriptor.position);
    m_originalPosition = enemyDescriptor.position;
    m_sprite.setPosition(enemyDescriptor.position);

    m_enemySight.setSize({ m_sightArea.x, m_sightArea.y });
    m_enemySight.setOrigin(m_enemySight.getOrigin().x, m_enemySight.getSize().y / 2.f);
    m_enemySight.setFillColor(sf::Color(0, 0, 255, 50)); // Semi-transparent blue
    m_enemySight.setOutlineColor(sf::Color::Blue);
    m_enemySight.setOutlineThickness(1.0f);

    return true;
}

void Enemy::update(float deltaMilliseconds)
{
    // Important to update the sight sense
    updateSight();

    handleCollisions();

    handleState(deltaMilliseconds);

    float deltaSeconds = deltaMilliseconds / 1000.f;

    updateEnemyPosition(deltaSeconds);

    updateEnemySprite(deltaSeconds);

    for (auto& attack : m_attacks)
    {
        attack->update(deltaSeconds);
    }
}

void Enemy::render(sf::RenderWindow& window)
{
	window.draw(m_sprite);

    m_collider->render(window);

	//const sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();
	//sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
	//boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
	//boundsRect.setOutlineColor(sf::Color::Red);
	//boundsRect.setOutlineThickness(2.f);
	//boundsRect.setFillColor(sf::Color::Transparent);
	//window.draw(boundsRect);

    for (auto& attack : m_attacks)
    {
        attack->render(window);
    }

    window.draw(m_enemySight);
}

void Enemy::handleCollisions()
{
    // To-Do
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
        updateAnimation();
    }
}

void Enemy::updateAnimation()
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
    }

    if (m_animations->count(animationType))
    {
        m_currentAnimation = (*m_animations)[animationType];
        m_currentAnimation->reset();
    }
}

void Enemy::setSpeedForState()
{
    switch (m_currentState)
    {
    case EnemyState::Patrol:
        m_speed = m_patrolSpeed;
        break;
    case EnemyState::Chase:
        m_speed = m_chaseSpeed;
        break;
    case EnemyState::ReturnToOrigin:
        m_speed = m_patrolSpeed;
        break;
    default:
        m_speed = { 0.0f, 0.0f };
        break;
    }
}

void Enemy::updateEnemyPosition(float deltaSeconds) {
    // Move based on direction and speed
    m_position.x += m_direction.x * m_speed.x * deltaSeconds;
    m_position.y += m_direction.y * m_speed.y * deltaSeconds;

    if (m_movingRight) {
        m_sprite.setScale(1.0f, 1.0f);
    }
    else {
        m_sprite.setScale(-1.0f, 1.0f);
    }
    m_sprite.setPosition(m_position);
    // Sync collider position
    m_collider->setPosition(m_position);
    m_collider->setDirection(m_direction);
}

void Enemy::updateEnemySprite(float deltaSeconds)
{
    if (m_currentAnimation && !m_currentAnimation->getFrames().empty())
    {
        m_currentAnimation->update(deltaSeconds);
        m_sprite.setTexture(*m_currentAnimation->getCurrentFrame());
    }
    else
    {
        printf("Error: Current animation is not set or has no frames\n");
    }
}

void Enemy::updateSight()
{
    m_enemySight.setPosition(m_position);

    if (m_movingRight) // Facing left
    {
        m_enemySight.setSize({ m_sightArea.x, m_sightArea.y });
    }
    else // Facing right
    {
        m_enemySight.setSize({ -m_sightArea.x, m_sightArea.y });
    }
}

bool Enemy::isPlayerInSight() {
    return m_collisionManager->isPlayerInsideArea(m_enemySight.getGlobalBounds());
}

bool Enemy::isPlayerInArea() {
    return m_collisionManager->isPlayerInsideArea(m_patrolArea->getGlobalBounds());
}