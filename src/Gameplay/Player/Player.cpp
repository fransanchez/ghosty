#include <Core/AudioManager.h>
#include <Gameplay/Collectibles/Collectible.h>
#include <Gameplay/Collisions/Collider.h>
#include <Gameplay/Collisions/CollisionManager.h>
#include <Gameplay/Player/Player.h>
#include <Gameplay/AttackSystem/Attack.h>
#include <Render/AnimationType.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

Player::~Player() {
}

bool Player::init(const PlayerDescriptor& descriptor,
    Collider* collider,
    CollisionManager* collisionManager)
{
    if (!initEntity(descriptor.animations, descriptor.attacks, descriptor.maxLife, descriptor.position))
    {
        return false;
    }
    m_collider = collider;
    m_collisionManager = collisionManager;
    m_collisionManager->registerPlayer(this);
    setPosition(descriptor.position);
    setSpeed(descriptor.speed);
    setMovingRight(true);
    return true;
}

void Player::update(float deltaMilliseconds)
{
    // First always update player position before calling parent
    // to update sprites and other items
    float deltaSeconds = deltaMilliseconds / 1000.f;

    if (!isDead()) {

        if (shouldEndInvincibility()) {
            setInvincibility(false);
        }

        handleInput();

        handleCollisions();

        updatePlayerPosition(deltaSeconds);
    }

    Entity::update(deltaMilliseconds);
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
            updateAnimationType();
            AudioManager::getInstance()->playSoundEffect(SoundType::PlayerJump);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
    {
        if (!m_attackKeyPressed) {
            m_attackKeyPressed = true;
            

            // Use the weapon to attack
            if (!m_attacks.empty() && m_attacks[m_currentAttackIndex]->canAttack())
            {
                setIsAttacking(true);
                sf::Vector2f attackDirection = (m_sprite.getScale().x > 0.f) ? sf::Vector2f(1.f, 0.f) : sf::Vector2f(-1.f, 0.f);
                sf::Vector2f attackPosition = m_sprite.getPosition();

                m_attacks[m_currentAttackIndex]->attack(attackPosition, attackDirection);
                SoundType sound = m_attacks[m_currentAttackIndex]->getSoundType();
                AudioManager::getInstance()->playSoundEffect(sound);
            }
        }
    }
    else {
        m_attackKeyPressed = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_direction.x = -1.f * horizontalVelocity;
        setMovingRight(false);
        updateAnimationType();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {

        m_direction.x = 1.f * horizontalVelocity;
        setMovingRight(true);
        updateAnimationType();
    }

    if (m_direction == sf::Vector2f(0.f, 0.f) && m_isGrounded)
    {
        updateAnimationType();
    }
}

void Player::setGrounded(bool grounded) {

    m_isGrounded = grounded;
    if (grounded)
    {
        m_verticalVelocity = 0.0f;
    }
}

void Player::updateAnimationType()
{

    AnimationType desiredAnimationType = AnimationType::Idle;

    if (isInvincible()) {
        desiredAnimationType = AnimationType::Hurt;
    }
    else if (isDead()) {
        desiredAnimationType = AnimationType::Death;
    }
    else 
    {
        // Jumping
        if (!m_isGrounded) {
            desiredAnimationType = AnimationType::Jump;
        }
        else if (m_direction == sf::Vector2f(0.f, 0.f))
        {
            // Idle
            if (isAttacking()) {
                desiredAnimationType = AnimationType::Attack;
            }
            else {
                desiredAnimationType = AnimationType::Idle;
            }
        }
        else {
            // Movement
            if (isAttacking()) {
                desiredAnimationType = m_isRunning ? AnimationType::RunAttack : AnimationType::WalkAttack;
            }
            else
            {
                desiredAnimationType = m_isRunning ? AnimationType::Run : AnimationType::Walk;
            }
        }
    }

    setAnimationType(desiredAnimationType);
}

void Player::handleCollisions()
{
    handleScenarioCollisions();

    handleCollectibleCollisions();

    if (!isInvincible()) {
        handleHurtingCollisions();
    }
}

void Player::handleScenarioCollisions()
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

    if (m_collisionManager->isPlayerFallingToDeath()) {
        AudioManager::getInstance()->playSoundEffect(SoundType::PlayerDeath);
        setIsDead(true);
        updateAnimationType();
    }
}

void Player::handleHurtingCollisions()
{

    int damage = m_collisionManager->checkPlayerHurtingCollisions();

    if (damage > 0)
    {
        reduceLives(damage);

        if (getCurrentLives() == 0)
        {
            AudioManager::getInstance()->playSoundEffect(SoundType::PlayerDeath);
            setIsDead(true);
        }
        else
        {
            AudioManager::getInstance()->playSoundEffect(SoundType::PlayerHurt);
            setInvincibility(true);
        }
        updateAnimationType();
    }
}

void Player::handleCollectibleCollisions()
{
    Collectible* collectible = m_collisionManager->checkPlayerCollectibleCollision();
    if (collectible)
    {
        collectible->applyEffect(this);
    }
}