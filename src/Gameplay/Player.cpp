#include <Gameplay/Player.h>
#include <Render/AnimationType.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>


bool Player::init(const PlayerDescriptor& descriptor, const std::unordered_map<AnimationType, Animation>& animations)
{
    m_animations = animations;

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

    m_position.x += m_direction.x * m_speed.x * deltaSeconds;
    m_position.y += m_direction.y * m_speed.y * deltaSeconds;

    m_sprite.setPosition(m_position);

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

void Player::setAnimation(AnimationType animationType)
{

    if (m_currentAnimation && m_currentAnimation == &m_animations[animationType])
    {
        return;
    }

    if (m_animations.count(animationType))
    {
        m_currentAnimation = &m_animations[animationType];
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

void Player::handleInput()
{
    m_direction = { 0.f, 0.f };

    bool isRunning = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        m_direction.y = -1.f;
        setAnimation(isRunning ? AnimationType::Run : AnimationType::Walk);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        m_direction.y = 1.f;
        setAnimation(isRunning ? AnimationType::Run : AnimationType::Walk);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_direction.x = -1.f;
        m_sprite.setScale(-1.0f, 1.0f);
        setAnimation(isRunning ? AnimationType::Run : AnimationType::Walk);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {

        m_direction.x = 1.f;
        m_sprite.setScale(1.0f, 1.0f);
        setAnimation(isRunning ? AnimationType::Run : AnimationType::Walk);
    }

    if (m_direction == sf::Vector2f(0.f, 0.f))
    {
        m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);
        setAnimation(AnimationType::Idle);
    }
}

