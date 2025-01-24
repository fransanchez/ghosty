#pragma once

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <nlohmann/json.hpp>

class Collider : public GameObject
{
public:

    Collider(const sf::Vector2f& center, const sf::Vector2f& size, const sf::Vector2f& centerOffset);

    sf::Vector2f getCenterOffset() const;
    sf::Vector2f getCenter() const;

    void setSize(const sf::Vector2f& size);
    sf::Vector2f getSize() const;

    sf::FloatRect getBounds() const;
    void setPosition(const sf::Vector2f& newPosition) override;
    void update(float delta) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Vector2f m_size;
    sf::Vector2f m_centerOffset;

};
