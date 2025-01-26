#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/Enemy/Enemy.h>
#include <SFML/Graphics/RenderWindow.hpp>

Enemy::~Enemy()
{
    for (auto attack : m_attacks)
        delete attack;

    for (auto& [type, animation] : *m_animations)
        delete animation;

    delete m_animations;
}

bool Enemy::init(const EnemyDescriptor& enemyDescriptor,
    Collider* collider,
    CollisionManager* collisionManager)
{
    m_animations = enemyDescriptor.animations;
    m_attacks = enemyDescriptor.attacks;
    m_collider = collider;
    m_collisionManager = collisionManager;
    m_speed = enemyDescriptor.speed;
    m_sightRange = enemyDescriptor.sightRange;

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
    m_sprite.setPosition(enemyDescriptor.position);

    m_enemySight.setOrigin(m_enemySight.getOrigin().x, m_sprite.getLocalBounds().height / 2.f);
    m_enemySight.setSize({ m_sightRange, m_sprite.getGlobalBounds().height });
    m_enemySight.setFillColor(sf::Color(0, 0, 255, 50)); // Semi-transparent blue
    m_enemySight.setOutlineColor(sf::Color::Blue);
    m_enemySight.setOutlineThickness(1.0f);

    return true;
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

void Enemy::changeState(EnemyState newState)
{
    if (m_currentState != newState)
    {
        m_currentState = newState;
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