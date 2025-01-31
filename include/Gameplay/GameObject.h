#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf
{
	class RenderWindow;
}

class GameObject
{
	public:

		GameObject() = default;
		GameObject(float x, float y) : m_position{ x,y } {}
		virtual ~GameObject() = default; // IMPORTANT! If we don't define it as virtual, we will leak memory

		const sf::Vector2f& getPosition() const { return m_position; }
		sf::Vector2f& getPosition() { return m_position; }
		virtual void setPosition(const sf::Vector2f& newPosition) { m_position = newPosition; }
		sf::Vector2f& getDirection() { return m_direction; }
		virtual void setDirection(const sf::Vector2f& newDirection) { m_direction = newDirection; }
		sf::Vector2f& getSpeed() { return m_speed; }
		virtual void setSpeed(const sf::Vector2f& newSpeed) { m_speed = newSpeed; }

		virtual void update(float delta) = 0;
		virtual void render(sf::RenderWindow& window) = 0;

	protected:

		sf::Vector2f m_speed{ 0.f, 0.f };
		sf::Vector2f m_position{ .0f, .0f };
		sf::Vector2f m_direction{ .0f, .0f };
};