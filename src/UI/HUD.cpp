#include <UI/HUD.h>
#include <Core/AssetManager.h>
#include <cstdio>
#include <Utils/Constants.h>

HUD::HUD() : m_maxLives(0), m_heartTexture{ nullptr } {}

HUD::~HUD() {

    for (auto* sprite : m_heartSprites)
    {
        delete sprite;
    }
    m_heartSprites.clear();

    delete m_heartTexture;
    m_heartTexture = nullptr;
}

bool HUD::init(int maxLives)
{
    m_heartTexture = AssetManager::getInstance()->loadTexture(ASSETS_LIVES_PATH);
    if (!m_heartTexture)
    {
        printf("Error: Could not load heart texture from %s\n", ASSETS_LIVES_PATH);
        return false;
    }

    m_heartTexture->setSmooth(true);
    m_maxLives = maxLives;
    
    m_heartSprites.clear();
    for (int i = 0; i < maxLives; ++i)
    {
        sf::Sprite* sprite = new sf::Sprite(*m_heartTexture);;
        sprite->setScale(1.5f, 1.5f);
        sprite->setPosition(10.f + i * (sprite->getGlobalBounds().width + 5.f), 10.f);
        m_heartSprites.push_back(sprite);
    }
    //for (int i = 0; i < maxLives; ++i)
    //{
    //    sf::Sprite* heartSprite = 
    //    heartSprite->setPosition(10.f + i * 40.f, 10.f); // Position hearts with spacing
    //    m_heartSprites.push_back(heartSprite);
    //}

    return true;
}

void HUD::update(int currentLives)
{
    for (int i = 0; i < m_maxLives; ++i)
    {
        if (i < currentLives)
        {
            m_heartSprites[i]->setColor(sf::Color::White);
        }
        else
        {
            m_heartSprites[i]->setColor(sf::Color(255, 255, 255, 50));
        }
    }
}

void HUD::render(sf::RenderWindow& window)
{
    for (const auto* heartSprite : m_heartSprites)
    {
        window.draw(*heartSprite);
    }
}
