#pragma once

#include <cstdint>
#include <UI/UIScreen.h>

namespace sf
{
    class Texture;
    class Sprite;
}

class UIScreenMainMenu : public UIScreen
{
    public:
        ~UIScreenMainMenu() override;
        void init(uint32_t windowWidth, uint32_t windowHeight) override;
        void deInit() override;
        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;
        void handleMouseClick(sf::Vector2f mousePosition) override;

    private:
        void updateButtonEffect(float deltaMilliseconds);
        void updateFadeInFadeOut(float deltaMilliseconds);

        sf::Texture* m_backgroundTexture{ nullptr };
        sf::Sprite m_backgroundSprite;

        sf::Texture* m_startButtonTexture{ nullptr };
        sf::Sprite m_startButtonSprite;

        float m_alpha{ 0.f };
        bool m_fadingIn{ false };
        bool m_fadingOut{ false };
        bool m_buttonClicked{ false };
        float m_buttonClickAlpha{ 255.f };
        float m_buttonFadeTimer{ 0.f };
        float m_fadeSpeed{ 100.f };
};