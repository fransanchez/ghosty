#include <UI/UIManager.h>
#include <UI/UIScreen.h>
#include <SFML/Graphics/RenderWindow.hpp>

void UIManager::show(UIScreen& screen)
{
	m_currentUIScreen = &screen;
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
