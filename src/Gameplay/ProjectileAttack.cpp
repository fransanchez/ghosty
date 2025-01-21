#include <Gameplay/ProjectileAttack.h>

ProjectileAttack::ProjectileAttack(const Animation& animation, float speed, float lifetime)
    : m_animation(animation), m_speed(speed), m_lifetime(lifetime)
{
    m_animation.reset();
}

void ProjectileAttack::activate(const sf::Vector2f& position, const sf::Vector2f& direction)
{
    m_sprite.setPosition(position);
    m_direction = direction;
    m_elapsedTime = 0.0f;
    m_finished = false;
    m_animation.reset();
}

void ProjectileAttack::update(float deltaTime)
{
    if (m_finished) return;

    m_elapsedTime += deltaTime;
    if (m_elapsedTime >= m_lifetime)
    {
        m_finished = true;
        return;
    }

    sf::Vector2f movement = m_direction * m_speed * deltaTime;
    m_sprite.move(movement);

    m_animation.update(deltaTime);
    m_sprite.setTexture(*m_animation.getCurrentFrame());
}

void ProjectileAttack::render(sf::RenderWindow& window)
{
    if (!m_finished)
        window.draw(m_sprite);
}

bool ProjectileAttack::isFinished() const
{
    return m_finished;
}
