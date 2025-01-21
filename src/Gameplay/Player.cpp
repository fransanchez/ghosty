#include <Gameplay/Player.h>
#include <Gameplay/ProjectileAttack.h>
#include <Render/AnimationType.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

bool Player::init(const PlayerDescriptor& descriptor,
    const std::unordered_map<AnimationType, Animation>& animations)
{
    m_animations = move(animations);

    if (m_animations.count(AnimationType::Idle))
    {
        m_currentAnimation = &m_animations[AnimationType::Idle];
    }
    else
    {
        printf("Error: Idle animation is missing\n");
        return false;
    }

    setPosition(descriptor.position);
    m_sprite.setPosition(descriptor.position);
    m_speed = descriptor.speed;
    return true;
}

void Player::update(float deltaMilliseconds)
{

    handleInput();

    float deltaSeconds = deltaMilliseconds / 1000.f;

    if (!m_isGrounded)
    {
        m_verticalVelocity += m_gravity * deltaSeconds;

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

void Player::render(sf::RenderWindow& window)
{

    window.draw(m_sprite);

    sf::FloatRect bounds = m_sprite.getGlobalBounds();
    sf::RectangleShape debugRect(sf::Vector2f(bounds.width, bounds.height));
    debugRect.setPosition(bounds.left, bounds.top);
    debugRect.setOutlineColor(sf::Color::Red);
    debugRect.setOutlineThickness(1.0f);
    debugRect.setFillColor(sf::Color::Transparent);

    window.draw(debugRect);
}


void Player::handleInput()
{
    m_direction = { 0.f, 0.f };

    bool isRunning = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        if (m_isGrounded)
        {
            m_verticalVelocity = JUMP_INITIAL_VELOCITY;
            m_isGrounded = false;
            setAnimation(isRunning);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
    {   
        m_isAttacking = true;
        setAnimation(isRunning);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        m_direction.y = -1.f;
        setAnimation(isRunning);

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        m_direction.y = 1.f;
        setAnimation(isRunning);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_direction.x = -1.f;
        m_sprite.setScale(-1.0f, 1.0f);
        setAnimation(isRunning);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {

        m_direction.x = 1.f;
        m_sprite.setScale(1.0f, 1.0f);
        setAnimation(isRunning);
    }

    if (m_direction == sf::Vector2f(0.f, 0.f) && m_isGrounded)
    {
        m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);
        setAnimation(isRunning);
    }
}

void Player::resetVerticalVelocity()
{
    m_verticalVelocity = 0.0f;
}

void Player::setAnimation(bool isRunning)
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
            desiredAnimationType = isRunning ? AnimationType::RunAttack : AnimationType::WalkAttack;
        }
        else
        {
            desiredAnimationType = isRunning ? AnimationType::Run : AnimationType::Walk;
        }
    }

    // Check if it's not the same as the one desired, otherwise return
    if (m_currentAnimation && m_currentAnimation == &m_animations[desiredAnimationType])
    {
        return;
    }

    if (m_animations.count(desiredAnimationType))
    {
        m_currentAnimation = &m_animations[desiredAnimationType];
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
