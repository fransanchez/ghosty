#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <SFML/Graphics/Shape.hpp>
#include <unordered_map>

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
        const std::vector<sf::Shape*>& getEnemyPatrolAreasShapes() const;
        const std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>>& getPlayerSpawnPoints() const;
        const std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>>& getEnemySpawnPoints() const;
        const std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>>& getCollectiblesSpawnPoints() const;
        const std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>> getPlayerSpawnPoint() const;
        sf::Shape* getEndOfLevelShape() const;


        float getWidth() const;
        float getHeight() const;

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
        ObjectLayer* m_enemyPatrolAreasLayer{ nullptr };
        ObjectLayer* m_collectiblesSpawnsLayer{ nullptr };
        ObjectLayer* m_endOfLevelLayer{ nullptr };
};
