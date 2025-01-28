#include <UI/UIScreenPlaying.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Utils/Constants.h>

UIScreenPlaying::~UIScreenPlaying()
{
    deInit();
}

void UIScreenPlaying::init(sf::RenderWindow* window)
{
    m_window = window;
    sf::Vector2u windowSize = m_window->getSize();

    m_world = new World();
    if (!m_world->load(windowSize.x, windowSize.y))
    {
        printf("Error: Could not load the World.\n");
        delete m_world;
        m_world = nullptr;
    }
}

void UIScreenPlaying::deInit()
{
    if (m_world)
    {
        m_window->setView(m_window->getDefaultView());
        m_world->unload();
        delete m_world;
        m_world = nullptr;
    }
    m_window = nullptr;
}

void UIScreenPlaying::update(float deltaMilliseconds)
{
    if (m_world)
    {
        m_world->update(deltaMilliseconds);
        if (m_world->isPlayerDead())
        {
            m_window->setView(m_window->getDefaultView());
            m_nextGameState = Game::GameState::GameOver;
        }
    }
}

void UIScreenPlaying::render(sf::RenderWindow& window)
{
    if (m_world)
    {
        m_world->render(window);
    }
}

void UIScreenPlaying::handleMouseClick(sf::Vector2f mousePosition)
{
    // No need to do anything here, gameplay doesn't need mouse
}