#include <Core/AssetManager.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Mouse.hpp>
#include <UI/UIScreenMainMenu.h>
#include <Utils/Constants.h>

UIScreenMainMenu::~UIScreenMainMenu() 
{
    deInit();
}

void UIScreenMainMenu::init(uint32_t windowWidth, uint32_t windowHeight)
{
    m_backgroundTexture = AssetManager::getInstance()->loadTexture(TITLE_SCREEN_IMAGE_PATH);
 
    m_backgroundSprite.setTexture(*m_backgroundTexture);

    sf::Vector2u textureSize = m_backgroundTexture->getSize();
    sf::Vector2u windowSize = { windowWidth, windowHeight };

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    m_backgroundSprite.setScale(scaleX, scaleY);

    m_startButtonTexture = AssetManager::getInstance()->loadTexture(START_BUTTON_IMAGE_PATH);

    m_startButtonSprite.setTexture(*m_startButtonTexture);
    float buttonWidth = m_startButtonSprite.getGlobalBounds().width;
    float buttonHeight = m_startButtonSprite.getGlobalBounds().height;
    m_startButtonSprite.setPosition((windowWidth - buttonWidth) / 2, windowHeight - buttonHeight - 50);

    m_alpha = 0.f;
    m_fadingIn = true;
    m_fadingOut = false;
    m_buttonClicked = false;
    m_buttonClickAlpha = 255.f;
    m_buttonFadeTimer = 0.f;
}

void UIScreenMainMenu::deInit()
{
    delete m_backgroundTexture;
    m_backgroundTexture = nullptr;
    delete m_startButtonTexture;
    m_startButtonTexture = nullptr;
}

void UIScreenMainMenu::update(float deltaMilliseconds)
{
    updateButtonEffect(deltaMilliseconds);
    updateFadeInFadeOut(deltaMilliseconds);
}

void UIScreenMainMenu::render(sf::RenderWindow& window)
{
    window.draw(m_backgroundSprite);
    window.draw(m_startButtonSprite);
}

void UIScreenMainMenu::handleMouseClick(sf::Vector2f mousePosition)
{
    if (m_startButtonSprite.getGlobalBounds().contains(mousePosition))
    {
        m_buttonClicked = true;
        m_buttonFadeTimer = 0.f;
    }
}

void UIScreenMainMenu::updateButtonEffect(float deltaMilliseconds)
{
    if (m_buttonClicked)
    {
        m_buttonFadeTimer += deltaMilliseconds;
        float clickFadeDuration = 200.f;

        if (m_buttonFadeTimer < clickFadeDuration / 2)
        {
            m_buttonClickAlpha = 150.f;
        }
        else if (m_buttonFadeTimer < clickFadeDuration)
        {
            m_buttonClickAlpha = 255.f;
        }
        else
        {
            m_buttonClicked = false;
            m_buttonFadeTimer = 0.f;
            m_fadingOut = true;
        }

        sf::Color buttonColor = m_startButtonSprite.getColor();
        buttonColor.a = static_cast<sf::Uint8>(std::round(m_buttonClickAlpha));
        m_startButtonSprite.setColor(buttonColor);
    }
}

void UIScreenMainMenu::updateFadeInFadeOut(float deltaMilliseconds)
{
    if (m_fadingIn)
    {
        m_alpha += m_fadeSpeed * (deltaMilliseconds / 1000.0f);
        if (m_alpha >= 255)
        {
            m_alpha = 255;
            m_fadingIn = false;
        }
    }
    else if (m_fadingOut)
    {
        // Fading out to next state
        m_alpha -= m_fadeSpeed * (deltaMilliseconds / 1000.0f);
        if (m_alpha <= 0)
        {
            m_alpha = 0;
            m_fadingOut = false;
            m_nextGameState = Game::GameState::Playing;
        }
    }

    sf::Color spriteColor = m_backgroundSprite.getColor();
    spriteColor.a = static_cast<sf::Uint8>(std::round(m_alpha));
    m_backgroundSprite.setColor(spriteColor);

    spriteColor = m_startButtonSprite.getColor();
    spriteColor.a = static_cast<sf::Uint8>(std::round(m_alpha));
    m_startButtonSprite.setColor(spriteColor);
}