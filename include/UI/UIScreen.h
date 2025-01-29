#pragma once

#include <Core/Game.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdint>

class UIScreen
{
	public:
		virtual ~UIScreen() = default;

		virtual void init(sf::RenderWindow* window) = 0;
		virtual void unload() = 0;
		virtual void update(float deltaMilliseconds) = 0;
		virtual void render(sf::RenderWindow& window) = 0;
		virtual void handleMouseClick(sf::Vector2f mousePosition) = 0;

		Game::GameState getNextGameState() const { return m_nextGameState; }
		void clearNextGameState() { m_nextGameState = Game::GameState::None; }
	protected:

		sf::RenderWindow* m_window{ nullptr };
		Game::GameState m_nextGameState{ Game::GameState::None };

};