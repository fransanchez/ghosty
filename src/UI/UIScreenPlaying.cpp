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

    m_transitioningToNextScreen = false;
    m_screenTransitionDelayTimer = 0.f;
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

        if (!m_transitioningToNextScreen &&
            (m_world->isPlayerDead() || m_world->isPlayerAtEndLevel()))
        {
            m_transitioningToNextScreen = true;
            m_screenTransitionDelayTimer = 0.f;
        }

        if (m_transitioningToNextScreen) {
            updateTransitionToNextLevel(deltaMilliseconds);
        }
    }
}

void UIScreenPlaying::updateTransitionToNextLevel(float deltaMilliseconds) {
    float deltaSeconds = deltaMilliseconds / 1000.f;

    m_screenTransitionDelayTimer += deltaSeconds;
    m_fadeAlpha += FADEOUT_SPEED * deltaSeconds;

    if (m_fadeAlpha > 255.f) m_fadeAlpha = 255.f;

    if (m_screenTransitionDelayTimer >= SCREEN_TRANSITION_DELAY)
    {
        m_window->setView(m_window->getDefaultView());
        if (m_world->isPlayerDead()) {
            m_nextGameState = Game::GameState::GameOver;
        }
        else {
            m_nextGameState = Game::GameState::Victory;
        }
        unload();
    }
}

void UIScreenPlaying::render(sf::RenderWindow& window)
{
    if (m_world)
    {
        m_world->render(window);
    }
    if (m_transitioningToNextScreen)
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