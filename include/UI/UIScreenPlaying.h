#pragma once

#include <cstdint>
#include <UI/UIScreen.h>

class World;

class UIScreenPlaying : public UIScreen
{
    public:
        ~UIScreenPlaying() override;
        void init(uint32_t windowWidth, uint32_t windowHeight) override;
        void deInit() override;
        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;
        void handleMouseClick(sf::Vector2f mousePosition) override;

    private:
        World* m_world{ nullptr };
        uint32_t m_windowWidth{ 0 };
        uint32_t m_windowHeight{ 0 };
};
