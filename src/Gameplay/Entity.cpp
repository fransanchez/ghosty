#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/Entity.h>
#include <Gameplay/AttackSystem/Attack.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdio>

Entity::~Entity()
{
    for (auto& attack : m_attacks) {
        delete attack;
    }
    m_attacks.clear();

    if (m_animations)
    {
        for (auto& [type, animation] : *m_animations)
        {
            delete animation;
        }
        delete m_animations;
        m_animations = nullptr;
    }
    m_currentAnimation = nullptr;
}

bool Entity::initEntity(
    std::unordered_map<AnimationType, Animation*>* animations,
    std::vector<Attack*> attacks,
    int maxLife,
    const sf::Vector2f& position)
{
    m_animations = animations;
    m_attacks = attacks;
    m_life = EntityLife(maxLife);

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
    m_sprite.setPosition(position);

    return true;
}

void Entity::update(float deltaMilliseconds)
{
    // This is always called after update from player or enemy
    if (!isDead()) {
        updateAttacks(deltaMilliseconds);

        if (m_isAttacking && m_currentAnimation->isFinished())
        {
            m_isAttacking = false;
        }
    }
    else
    {
        if (m_currentAnimation->isFinished()) {
            m_markedForDestruction = true;
        }
    }
    updateAnimationSprite(deltaMilliseconds);

    // Call parent to update colliders positions
    Collisionable::update(deltaMilliseconds);
}

void Entity::updateAnimationSprite(float deltaMilliseconds)
{
    if (m_currentAnimation && !m_currentAnimation->getFrames().empty())
    {
        m_currentAnimation->update(deltaMilliseconds);
        if (m_movingRight) {
            m_sprite.setScale(1.0f, 1.0f);
        }
        else {
            m_sprite.setScale(-1.0f, 1.0f);
        }
        m_sprite.setPosition(m_position);
        m_sprite.setTexture(*m_currentAnimation->getCurrentFrame());
    }
    else
    {
        printf("Error: Current animation is not set or has no frames\n");
    }
}

void Entity::updateAttacks(float deltaMilliseconds)
{
    for (auto& attack : m_attacks)
    {
        attack->update(deltaMilliseconds);
    }
}

void Entity::render(sf::RenderWindow& window)
{
    window.draw(m_sprite);

    for (auto& attack : m_attacks)
    {
        attack->render(window);
    }

    Collisionable::render(window);
}

void Entity::setAnimationType(AnimationType desiredAnimationType)
{
    if (m_currentAnimation && m_currentAnimation == (*m_animations)[desiredAnimationType])
    {
        return;
    }

    if (m_animations->count(desiredAnimationType))
    {
        m_currentAnimation = (*m_animations)[desiredAnimationType];
        m_currentAnimation->reset();
    }
    else
    {
        printf("Animation not found\n");
        m_currentAnimation = nullptr;
    }
}


int Entity::getCurrentLives()
{
    return m_life.getLife();
}

int Entity::getMaxLives()
{
    return m_life.getMaxLife();
}

void Entity::reduceLives(int damage)
{
    m_life.subtractLife(damage);
}

bool Entity::shouldEndInvincibility() {
    return (m_isInvincible &&
        m_currentAnimation == (*m_animations)[AnimationType::Hurt] &&
        m_currentAnimation->isFinished());
}