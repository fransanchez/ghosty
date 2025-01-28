#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>

class HUD
{
public:
    HUD();
    ~HUD();

    bool init(int maxLives);
    void update(int currentLives);
    void render(sf::RenderWindow& window);

private:
    sf::Texture* m_heartTexture{ nullptr };
    std::vector<sf::Sprite*> m_heartSprites;
    int m_maxLives;
};
