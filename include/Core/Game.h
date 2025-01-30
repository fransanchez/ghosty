#pragma once

#include <cstdint>
#include <string>

namespace sf
{
	class RenderWindow;
}

class World;
class UIManager;
class UIScreen;

class Game
{
	public:
		enum class GameState
		{
			None,
			MainMenu,
			Playing,
			Paused,
			GameOver,
			Victory
		};

		struct GameCreateInfo
		{
			std::string gameTitle;
			uint32_t screenWidth;
			uint32_t screenHeight;
			uint32_t frameRateLimit;
			std::string windowIconPath;
		};

		~Game();

		bool init(GameCreateInfo& createInfo);

		bool isRunning() const;

		void update(uint32_t deltaMilliseconds);
		void render();
		void changeState(GameState newState);

	private:
		void updateState();

		UIManager* m_uiManager{ nullptr } ;
		sf::RenderWindow* m_window{ nullptr };
		GameState m_currentState = GameState::MainMenu;
};