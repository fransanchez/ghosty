#include <UI/UIScreenPlaying.h>
#include <Core/World.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Utils/Constants.h>

UIScreenPlaying::~UIScreenPlaying()
{
    deInit();
}

void UIScreenPlaying::init(uint32_t windowWidth, uint32_t windowHeight) 
{
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;

    m_world = new World();
    if (!m_world->load(windowWidth, windowHeight))
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
        m_world->unload();
        delete m_world;
        m_world = nullptr;
    }
}

void UIScreenPlaying::update(float deltaMilliseconds)
{
    if (m_world)
    {
        m_world->update(deltaMilliseconds);
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