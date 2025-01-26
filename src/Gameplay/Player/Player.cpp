#include <Gameplay/Collider.h>
#include <Gameplay/CollisionManager.h>
#include <Gameplay/Player/Player.h>
#include <Gameplay/AttackSystem/Attack.h>
#include <Render/AnimationType.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

Player::~Player() {
    m_collisionManager->unregisterPlayer();

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

bool Player::init(const PlayerDescriptor& descriptor,
    Collider* collider,
    CollisionManager* collisionManager)
{
    m_animations = descriptor.animations;
    m_attacks = descriptor.attacks;
    m_collider = collider;
    m_collisionManager = collisionManager;

    m_collisionManager->registerPlayer(m_collider);

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
    setPosition(descriptor.position);
    m_sprite.setPosition(descriptor.position);

    m_speed = descriptor.speed;
    return true;
}

void Player::update(float deltaMilliseconds)
{

    handleInput();

    handleCollisions();

    float deltaSeconds = deltaMilliseconds / 1000.f;

    updatePlayerPosition(deltaSeconds);

    updateSpriteSelection(deltaSeconds);

    for (auto& attack : m_attacks)
    {
        attack->update(deltaSeconds);
    }
}

void Player::render(sf::RenderWindow& window)
{

    window.draw(m_sprite);

    for (auto& attack : m_attacks)
    {
        attack->render(window);
    }

    m_collider->render(window);

 /*   sf::FloatRect bounds = m_sprite.getGlobalBounds();
    sf::RectangleShape debugRect(sf::Vector2f(bounds.width, bounds.height));
    debugRect.setPosition(bounds.left, bounds.top);
    debugRect.setOutlineColor(sf::Color::Red);
    debugRect.setOutlineThickness(1.0f);
    debugRect.setFillColor(sf::Color::Transparent);

    window.draw(debugRect);*/
}

void Player::updatePlayerPosition(float deltaSeconds)
{
    if (!m_isGrounded)
    {
        m_verticalVelocity += GRAVITY * deltaSeconds;

        const float maxFallSpeed = 1000.0f;
        if (m_verticalVelocity > maxFallSpeed)
        {
            m_verticalVelocity = maxFallSpeed;
        }
    }
    else
    {
        m_verticalVelocity = 0.0f;
    }

    m_position.x += m_direction.x * m_speed.x * deltaSeconds;
    m_position.y += m_verticalVelocity * deltaSeconds;

    m_sprite.setPosition(m_position);
    m_collider->setPosition(m_position);
    m_collider->setDirection(m_direction);
}

void Player::updateSpriteSelection(float deltaSeconds)
{
    if (m_currentAnimation && !m_currentAnimation->getFrames().empty())
    {
        m_currentAnimation->update(deltaSeconds);
        m_sprite.setTexture(*m_currentAnimation->getCurrentFrame());

        if (m_isAttacking && m_currentAnimation->isFinished())
        {
            m_isAttacking = false;
        }
    }
    else
    {
        printf("Error: Current animation is not set or has no frames\n");
    }
}

void Player::handleInput()
{
    m_direction = { 0.f, 0.f };

    m_isRunning = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

    float horizontalVelocity = m_isRunning ? RUN_VELOCITY : WALK_VELOCITY;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        if (m_isGrounded)
        {
            m_verticalVelocity = JUMP_INITIAL_VELOCITY;
            m_isGrounded = false;
            setAnimation();
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
    {
        if (!m_attackKeyPressed) {
            m_attackKeyPressed = true;
            

            // Use the weapon to attack
            if (!m_attacks.empty() && m_attacks[m_currentAttackIndex]->canAttack())
            {
                m_isAttacking = true;
                sf::Vector2f attackDirection = (m_sprite.getScale().x > 0.f) ? sf::Vector2f(1.f, 0.f) : sf::Vector2f(-1.f, 0.f);
                sf::Vector2f attackPosition = m_sprite.getPosition();

                m_attacks[m_currentAttackIndex]->attack(attackPosition, attackDirection, m_collisionManager); // To-Do: First attack for now
            }
        }
    }
    else {
        m_attackKeyPressed = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        //m_direction.y = -1.f * horizontalVelocity;
        //setAnimation();

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        //m_direction.y = 1.f * horizontalVelocity;
        //setAnimation();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_direction.x = -1.f * horizontalVelocity;
        m_sprite.setScale(-1.0f, 1.0f);
        setAnimation();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {

        m_direction.x = 1.f * horizontalVelocity;
        m_sprite.setScale(1.0f, 1.0f);
        setAnimation();
    }

    if (m_direction == sf::Vector2f(0.f, 0.f) && m_isGrounded)
    {
        setAnimation();
    }
}

void Player::setGrounded(bool grounded) {

    m_isGrounded = grounded;
    if (grounded)
    {
        m_verticalVelocity = 0.0f;
    }
}

void Player::setAnimation()
{

    AnimationType desiredAnimationType = AnimationType::Idle;

    // Jumping
    if (!m_isGrounded) {
        desiredAnimationType = AnimationType::Jump;
    }
    else if (m_direction == sf::Vector2f(0.f, 0.f))
    {
        // Idle
        if (m_isAttacking) {
            desiredAnimationType = AnimationType::Attack;
        }
        else {
            desiredAnimationType = AnimationType::Idle;
        }
    }
    else {
        // Movement
        if (m_isAttacking) {
            desiredAnimationType = m_isRunning ? AnimationType::RunAttack : AnimationType::WalkAttack;
        }
        else
        {
            desiredAnimationType = m_isRunning ? AnimationType::Run : AnimationType::Walk;
        }
    }

    // Check if it's not the same as the one desired, otherwise return
    if (m_currentAnimation && m_currentAnimation == (*m_animations)[desiredAnimationType])
    {
        return;
    }

    if (m_animations->count(desiredAnimationType))
    {
        m_currentAnimation = (*m_animations)[desiredAnimationType];
        if (m_currentAnimation->getFrames().empty())
        {
            printf("Animation has no frames\n");
            m_currentAnimation = nullptr;
        }
        else
        {
            m_currentAnimation->reset();
        }
    }
    else
    {
        printf("Animation not found\n");
        m_currentAnimation = nullptr;
    }
}

void Player::handleCollisions()
{
    if (!m_collider || !m_collisionManager)
        return;

    if (m_verticalVelocity >= 0) {
        m_isGrounded = m_collisionManager->checkIsGrounded(m_collider);
    }

    auto wallCollision = m_collisionManager->checkWalls(m_collider);

    if (wallCollision.collided)
    {
        if (wallCollision.leftCollision && m_direction.x < 0.f)
        {
            m_direction.x = 0.f;
        }
        else if (wallCollision.rightCollision && m_direction.x > 0.f)
        {
            m_direction.x = 0.f;
        }

        if (wallCollision.topCollision && m_verticalVelocity < 0.f)
        {
            m_verticalVelocity = 0.f;
        }
    }
}