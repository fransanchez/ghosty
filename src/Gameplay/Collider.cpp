#include <Gameplay/Collider.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


Collider::Collider(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Vector2f& centerOffset)
    : m_size(size), m_centerOffset(centerOffset)
{
    m_position = position + centerOffset;
}

void Collider::setPosition(const sf::Vector2f& position) {
    sf::Vector2f relativeOffset;
    if (m_direction.x < 0) {
        relativeOffset.x = m_centerOffset.x;
    }
    else {
        relativeOffset.x = -m_centerOffset.x;
    }

    if (m_direction.y < 0) {
        relativeOffset.y = m_centerOffset.y;
    }
    else {
        relativeOffset.y = -m_centerOffset.y;
    }
    m_position = position + relativeOffset;
}

sf::Vector2f Collider::getCenterOffset() const
{
    return m_centerOffset;
}
sf::Vector2f Collider::getCenter() const
{
    return m_position;
}

void Collider::setSize(const sf::Vector2f& size)
{
    m_size = size;
}

sf::Vector2f Collider::getSize() const
{
    return m_size;
}

sf::FloatRect Collider::getBounds() const
{
    return sf::FloatRect(
        m_position.x - m_size.x / 2.f,
        m_position.y - m_size.y / 2.f,
        m_size.x,
        m_size.y
    );
}

void Collider::update(float delta)
{
}

void Collider::render(sf::RenderWindow& window)
{
    sf::RectangleShape rect(m_size);
    rect.setOrigin(m_size.x / 2.f, m_size.y / 2.f);
    rect.setPosition(m_position);
    rect.setOutlineColor(sf::Color::Green);
    rect.setOutlineThickness(1.f);
    rect.setFillColor(sf::Color::Transparent);
    window.draw(rect);
}
