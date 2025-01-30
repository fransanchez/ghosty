#pragma once

#include <UI/UIScreen.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class UIScreenStaticBackground : public UIScreen
{
public:
    UIScreenStaticBackground(char* backgroundPath);
    ~UIScreenStaticBackground() override;
    void init(sf::RenderWindow* window) override;
    void unload() override;
    void update(float deltaMilliseconds) override;
    void render(sf::RenderWindow& window) override;
    void handleMouseClick(sf::Vector2f mousePosition) override;

private:
    static constexpr float FADE_SPEED = 100.0f;
    static constexpr float WAIT_DURATION = 3000.0f;

    sf::Texture* m_backgroundTexture{ nullptr };
    sf::Sprite m_backgroundSprite;

    bool m_fadingIn{ false };
    bool m_fadingOut{ false };
    float m_alpha{ 0 };
    float m_timer{ 0 };
    char* m_backgroundPath;
};
