#include <UI/UIScreenGameOver.h>
#include <Core/AssetManager.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Utils/Constants.h>

UIScreenGameOver::~UIScreenGameOver()
{
    unload();
}

void UIScreenGameOver::init(sf::RenderWindow* window)
{
    m_window = window;

    m_backgroundTexture = AssetManager::getInstance()->loadTexture(GAME_OVER_SCREEN_IMAGE_PATH);
    if (!m_backgroundTexture)
    {
        printf("Error loading Game Over texture\n");
        return;
    }

    m_backgroundSprite.setTexture(*m_backgroundTexture);

    sf::Vector2u textureSize = m_backgroundTexture->getSize();
    sf::Vector2u windowSize = m_window->getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    m_backgroundSprite.setScale(scaleX, scaleY);
    sf::Color spriteColor = m_backgroundSprite.getColor();
    spriteColor.a = 0.f;
    m_backgroundSprite.setColor(spriteColor);

    m_fadingIn = true;
    m_fadingOut = false;
    m_alpha = 0;
    m_timer = 0.0f;
}

void UIScreenGameOver::unload()
{
    m_window = nullptr;
    m_backgroundTexture = nullptr;
}

void UIScreenGameOver::update(float deltaMilliseconds)
{
    if (m_fadingIn)
    {
        m_alpha += FADE_SPEED * (deltaMilliseconds / 1000.0f);
        if (m_alpha >= 255)
        {
            m_alpha = 255;
            m_fadingIn = false;
        }
    }
    else if (!m_fadingOut)
    {
        // Start fade out after 3 seconds automatically if player doesn't click
        m_timer += deltaMilliseconds;
        if (m_timer >= WAIT_DURATION)
        {
            m_fadingOut = true;
        }
    }
    else if (m_fadingOut)
    {
        m_alpha -= FADE_SPEED * (deltaMilliseconds / 1000.0f);
        if (m_alpha <= 0)
        {
            m_alpha = 0;
            m_fadingOut = false;
            unload();
            m_nextGameState = Game::GameState::MainMenu;
        }
    }

    sf::Color spriteColor = m_backgroundSprite.getColor();
    spriteColor.a = static_cast<sf::Uint8>(std::round(m_alpha));
    m_backgroundSprite.setColor(spriteColor);
}

void UIScreenGameOver::render(sf::RenderWindow& window)
{
    window.draw(m_backgroundSprite);
}

void UIScreenGameOver::handleMouseClick(sf::Vector2f mousePosition)
{
    if (!m_fadingOut)
    {
        m_fadingOut = true;
    }
}
