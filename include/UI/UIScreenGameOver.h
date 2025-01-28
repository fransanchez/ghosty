#pragma once

#include <UI/UIScreen.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class UIScreenGameOver : public UIScreen
{
public:
    ~UIScreenGameOver() override;
    void init(sf::RenderWindow* window) override;
    void deInit() override;
    void update(float deltaMilliseconds) override;
    void render(sf::RenderWindow& window) override;
    void handleMouseClick(sf::Vector2f mousePosition) override;

private:
    static constexpr float FADE_SPEED = 100.0f;
    static constexpr float WAIT_DURATION = 3000.0f;

    sf::Texture* m_backgroundTexture{ nullptr };
    sf::Sprite m_backgroundSprite;

    bool m_fadingIn;
    bool m_fadingOut;
    float m_alpha;
    float m_timer;
};
