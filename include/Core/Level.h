#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <SFML/Graphics/Shape.hpp>

namespace sf
{
    class RenderWindow;
    class Sprite;
    class Texture;
}

namespace tmx
{
    class Map;
    class ImageLayer;
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

        const std::vector<sf::Shape*>& getFloorsCollisionShapes() const;
        const std::vector<sf::Shape*>& getWallsCollisionShapes() const;
        const std::vector<sf::Shape*>& getPlayerSpawnPoints() const;
        const std::vector<sf::Shape*>& getEnemySpawnPoints() const;

    private:
        tmx::Map* m_map{ nullptr };
        std::vector<std::pair<sf::VertexArray, sf::Texture*>> m_imageLayers;
        MapLayer* m_decorations{ nullptr };
        MapLayer* m_fillers{ nullptr };
        MapLayer* m_ground{ nullptr };
        ObjectLayer* m_floorsCollisionLayer{ nullptr };
        ObjectLayer* m_wallsCollisionLayer{ nullptr };
        ObjectLayer* m_playerSpawnsLayer{ nullptr };
        ObjectLayer* m_enemySpawnsLayer{ nullptr };
};
