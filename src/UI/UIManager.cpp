#include <Core/Game.h>
#include <UI/UIManager.h>
#include <UI/UIScreen.h>
#include <SFML/Graphics/RenderWindow.hpp>

UIManager::~UIManager()
{
	for (auto& [state, screen] : m_screens)
		delete screen;
	m_screens.clear();
	m_currentUIScreen = nullptr;
}

void UIManager::update(float deltaMilliseconds)
{
	if (m_currentUIScreen != nullptr) {
		m_currentUIScreen->update(deltaMilliseconds);
	}
}

void UIManager::render(sf::RenderWindow& window)
{
	if (m_currentUIScreen != nullptr) {
		m_currentUIScreen->render(window);
	}
}

void UIManager::registerScreen(Game::GameState state, UIScreen* screen, uint32_t windowWidth, uint32_t windowHeight)
{
	m_screens[state] = screen;
	m_screens[state]->init(windowWidth, windowHeight);
}

void UIManager::setActiveScreen(Game::GameState state)
{
	if (m_screens.find(state) != m_screens.end())
		m_currentUIScreen = m_screens[state];
}

UIScreen* UIManager::getActiveScreen()
{
	return m_currentUIScreen;
}