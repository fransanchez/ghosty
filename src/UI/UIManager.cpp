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

void UIManager::registerScreen(Game::GameState state, UIScreen* screen)
{
	m_screens[state] = screen;
}

void UIManager::setActiveScreen(Game::GameState state, sf::RenderWindow* window)
{
	if (m_screens.find(state) != m_screens.end())
	{
		m_screens[state]->init(window);
		m_currentUIScreen = m_screens[state];
	}
}

UIScreen* UIManager::getActiveScreen()
{
	return m_currentUIScreen;
}