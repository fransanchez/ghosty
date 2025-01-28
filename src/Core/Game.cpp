#include <cassert>
#include <Core/Game.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <UI/UIManager.h>
#include <UI/UIScreenMainMenu.h>
#include <Utils/GameConfigLoader.h>


bool Game::init(GameCreateInfo& createInfo)
{
	assert(m_window == nullptr && m_world == nullptr && "Game is already initialized, we are about to leak memory");

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
	m_mainMenu = new UIScreenMainMenu();
	m_mainMenu->init();
	m_uiManager->show(*m_mainMenu);
	m_world = new World();
	const bool loadOk = m_world->load(createInfo.screenWidth, createInfo.screenHeight);

	return loadOk;
}

Game::~Game()
{
	// To-Do: make sure m_world is unloaded()
	delete m_world;
	delete m_mainMenu;
	delete m_uiManager;
	delete m_window;
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
			m_world->unload();
		}
	}

	// Update scene here
	m_world->update(deltaMilliseconds);

	m_uiManager->update(deltaMilliseconds);
}

void Game::render()
{
	m_window->clear();

	m_world->render(*m_window);

	m_uiManager->render(*m_window);

	m_window->display();
}