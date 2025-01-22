#include <Core/AssetManager.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <UI/UIScreenMainMenu.h>



void UIScreenMainMenu::init()
{
	sf::Texture* zombieTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Enemies/zombie.png");
	m_logo = new sf::RectangleShape();
	m_logo->setTexture(zombieTexture);
	m_logo->setSize(sf::Vector2f(200.f, 100.f));
	m_logo->setPosition(300.f, 250.f);
}

void UIScreenMainMenu::deInit() {

}

void UIScreenMainMenu::update(float deltaMilliseconds)
{

}

void UIScreenMainMenu::render(sf::RenderWindow& window)
{
	window.draw(*m_logo);
}