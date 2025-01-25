#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/Collider.h>
#include <Gameplay/Collisionable.h>
#include <Gameplay/CollisionManager.h>
#include <Gameplay/Enemy/Enemy.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

bool Enemy::init(const EnemyDescriptor& enemyDescriptor,
    Collider* collider,
    CollisionManager* collisionManager)
{
    m_animations = enemyDescriptor.animations;
    m_attacks = enemyDescriptor.attacks;
    m_collider = collider;
    m_collisionManager = collisionManager;

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
    m_speed = enemyDescriptor.speed;

    return true;
}

void Enemy::update(float deltaMilliseconds)
{
	m_sprite.setPosition(m_position);
}

void Enemy::render(sf::RenderWindow& window)
{

	window.draw(m_sprite);

    m_collider->render(window);

	const sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();
	sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));
	boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
	boundsRect.setOutlineColor(sf::Color::Red);
	boundsRect.setOutlineThickness(2.f);
	boundsRect.setFillColor(sf::Color::Transparent);
	window.draw(boundsRect);
}

void Enemy::handleCollisions()
{
    //To-Do
}
