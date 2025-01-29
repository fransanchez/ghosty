#pragma once

#include <cstdint>
#include <UI/UIScreen.h>

class World;

class UIScreenPlaying : public UIScreen
{
    public:
        ~UIScreenPlaying() override;
        void init(sf::RenderWindow* window) override;
        void unload() override;
        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;
        void handleMouseClick(sf::Vector2f mousePosition) override;

    private:
        static constexpr float DEATH_DELAY = 2.0f;
        static constexpr float FADEOUT_SPEED = 100.0f;

        World* m_world{ nullptr };
        bool m_transitioningToGameOver{ false };
        float m_deathDelayTimer{ 0.f };
        float m_fadeAlpha{ 0.f };
};
