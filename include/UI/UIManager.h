#pragma once

#include <unordered_map>
#include <SFML/Graphics/RenderWindow.hpp>

namespace sf
{
	class RenderWindow;
}

class UIScreen;
class Game;

class UIManager
{
	public:
		~UIManager();
		void registerScreen(Game::GameState state, UIScreen* screen, uint32_t windowWidth, uint32_t windowHeight);
		void setActiveScreen(Game::GameState state);
		void update(float deltaMilliseconds);
		void render(sf::RenderWindow& window);
		UIScreen* getActiveScreen();

	private:
		std::unordered_map<Game::GameState, UIScreen*> m_screens;
		UIScreen* m_currentUIScreen{ nullptr };
};