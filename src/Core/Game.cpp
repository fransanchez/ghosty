#include <Core/AssetManager.h>
#include <cassert>
#include <Core/Game.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <UI/UIManager.h>
#include <UI/UIScreenMainMenu.h>
#include <UI/UIScreenPlaying.h>
#include <UI/UIScreenStaticBackground.h>
#include <Utils/GameConfigLoader.h>



bool Game::init(GameCreateInfo& createInfo)
{
	assert(m_window == nullptr && "Game is already initialized");

	m_window = new sf::RenderWindow({ createInfo.screenWidth, createInfo.screenHeight }, createInfo.gameTitle);
	m_window->setFramerateLimit(createInfo.frameRateLimit);

	sf::Image icon;
	if (!createInfo.windowIconPath.empty() && icon.loadFromFile(createInfo.windowIconPath))
	{
		m_window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	}
	else {
		printf("Error: Could not load window icon\n");
	}

	m_uiManager = new UIManager();
	m_uiManager->registerScreen(GameState::MainMenu, new UIScreenMainMenu());
	m_uiManager->registerScreen(GameState::Playing, new UIScreenPlaying());
	m_uiManager->registerScreen(GameState::GameOver, new UIScreenStaticBackground(GAME_OVER_SCREEN_IMAGE_PATH));
	m_uiManager->registerScreen(GameState::Victory, new UIScreenStaticBackground(VICTORY_SCREEN_IMAGE_PATH));

	changeState(GameState::MainMenu);

	return true;
}

Game::~Game()
{
	delete m_uiManager;
	delete m_window;
	AssetManager::getInstance()->clear();
}

bool Game::isRunning() const 
{ 
	return m_window->isOpen(); 
}

void Game::update(uint32_t deltaMilliseconds)
{
	// Check if user closed the window
	for (auto event = sf::Event(); m_window->pollEvent(event);)
	{
		if (event.type == sf::Event::Closed)
		{
			m_window->close();
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			sf::Vector2f mousePosition = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
			if (m_uiManager->getActiveScreen())
			{
				m_uiManager->getActiveScreen()->handleMouseClick(mousePosition);
			}
		}
	}

	m_uiManager->update(deltaMilliseconds);

	updateState();
}

void Game::render()
{
	m_window->clear();
	m_uiManager->render(*m_window);
	m_window->display();
}

void Game::changeState(GameState newState)
{
	m_currentState = newState;
	m_uiManager->setActiveScreen(newState, m_window);
}

void Game::updateState() {
	UIScreen* activeScreen = m_uiManager->getActiveScreen();
	if (activeScreen && activeScreen->getNextGameState() != GameState::None)
	{
		changeState(activeScreen->getNextGameState());
		activeScreen->clearNextGameState();
	}
}