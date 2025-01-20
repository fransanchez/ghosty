#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <SFML/Graphics/Shape.hpp>

namespace sf
{
    class RenderWindow;
}

namespace tmx
{
    class Map;
}

class MapLayer;
class ObjectLayer;

class Level
{
    public:
        Level() = default;
        ~Level();

        bool load(const std::string& filePath);
        void unload();

        void update(uint32_t deltaMilliseconds);
        void render(sf::RenderWindow& window);

        const ObjectLayer* getCollisionLayer() const { return m_collisionLayer; }
        const std::vector<sf::Shape*>& getCollisionShapes() const;

    private:
        tmx::Map* m_map{ nullptr };
        MapLayer* m_layerZero{ nullptr };
        MapLayer* m_layerOne{ nullptr };
        MapLayer* m_layerTwo{ nullptr };
        ObjectLayer* m_collisionLayer{ nullptr };
};
