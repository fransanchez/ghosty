#include <UI/UIScreenPlaying.h>
#include <Core/World.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Utils/Constants.h>

UIScreenPlaying::~UIScreenPlaying()
{
    unload();
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

    m_transitioningToGameOver = false;
    m_deathDelayTimer = 0.f;
    m_fadeAlpha = 0.f;
}

void UIScreenPlaying::unload()
{
    if (m_world)
    {
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

        if (m_world->isPlayerDead() && !m_transitioningToGameOver)
        {
            m_transitioningToGameOver = true;
            m_deathDelayTimer = 0.f;
        }

        if (m_transitioningToGameOver)
        {
            m_deathDelayTimer += deltaMilliseconds / 1000.f;

            m_fadeAlpha += FADEOUT_SPEED * (deltaMilliseconds / 1000.f);
            if (m_fadeAlpha > 255.f) m_fadeAlpha = 255.f;

            if (m_deathDelayTimer >= DEATH_DELAY)
            {
                m_window->setView(m_window->getDefaultView());
                // To-Do - Fix memory nullpointer
                unload();
                m_nextGameState = Game::GameState::GameOver;
            }
        }
    }
}

void UIScreenPlaying::render(sf::RenderWindow& window)
{
    if (m_world)
    {
        m_world->render(window);
    }
    if (m_transitioningToGameOver)
    {
        // Same trick as we use to render the HUD in world. Change view -> paint -> restore view
        const sf::View originalView = window.getView();
        window.setView(window.getDefaultView());
        sf::RectangleShape fadeOverlay(sf::Vector2f(m_window->getSize()));
        fadeOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(m_fadeAlpha)));
        window.draw(fadeOverlay);
        window.setView(originalView);
    }
}

void UIScreenPlaying::handleMouseClick(sf::Vector2f mousePosition)
{
    // No need to do anything here, gameplay doesn't need mouse
}