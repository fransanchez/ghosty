#pragma once

#include <Render/ObjectLayer.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <tmxlite/Map.hpp>

class ObjectLayer : public sf::Drawable, public sf::Transformable
{
public:

    ObjectLayer(const tmx::Map& map, std::size_t layerID);

    const std::vector<sf::Shape*>& getShapes() const
    {
        return m_shapes;
    }
    const std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>>& getPoints() const
    {
        return m_points;
    }

private:

    sf::Shape* createShape(const tmx::Object& object);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>> m_points;
    std::vector<sf::Shape*> m_shapes;
};