#include <Core/Level.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/ImageLayer.hpp>
#include <Render/SFMLOrthogonalLayer.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

Level::~Level()
{
    unload();
}

bool Level::load(const std::string& filePath)
{
    m_map = new tmx::Map();
    if (!m_map->load(filePath))
    {
        return false;
    }

    // Load image layers manually
    const auto& layers = m_map->getLayers();
    for (const auto& layer : layers)
    {
        if (layer->getType() == tmx::Layer::Type::Image)
        {
            const auto& imageLayer = layer->getLayerAs<tmx::ImageLayer>();
            const auto& imagePath = imageLayer.getImagePath();

            sf::Texture* texture = new sf::Texture();
            if (!texture->loadFromFile(imagePath))
            {
                delete texture;
                printf("Error: Could not load image for ImageLayer: %s\n", imagePath.c_str());
                return false;
            }

            // Repeat background texture
            texture->setRepeated(true);

            auto mapBounds = m_map->getBounds();
            sf::Vector2f mapSize(mapBounds.width, mapBounds.height);

            // Create VertexArray to cover the full map
            sf::VertexArray vertices(sf::Quads, 4);

            vertices[0].position = sf::Vector2f(0.f, 0.f);
            vertices[1].position = sf::Vector2f(mapSize.x, 0.f);
            vertices[2].position = sf::Vector2f(mapSize.x, mapSize.y);
            vertices[3].position = sf::Vector2f(0.f, mapSize.y);

            vertices[0].texCoords = sf::Vector2f(0.f, 0.f);
            vertices[1].texCoords = sf::Vector2f(mapSize.x, 0.f);
            vertices[2].texCoords = sf::Vector2f(mapSize.x, mapSize.y);
            vertices[3].texCoords = sf::Vector2f(0.f, mapSize.y);

            m_imageLayers.emplace_back(vertices, texture);
        }
    }

    m_decorations = new MapLayer(*m_map, 1);
    m_fillers = new MapLayer(*m_map, 2);
    m_ground = new MapLayer(*m_map, 3);
    m_floorsCollisionLayer = new ObjectLayer(*m_map, 4);
    m_wallsCollisionLayer = new ObjectLayer(*m_map, 5);
    m_playerSpawnsLayer = new ObjectLayer(*m_map, 6);
    m_enemySpawnsLayer = new ObjectLayer(*m_map, 7);
    m_enemyPatrolAreasLayer = new ObjectLayer(*m_map, 8);


    return true;
}

void Level::unload()
{
    for (auto& imageLayer : m_imageLayers)
    {
        delete imageLayer.second;
    }
    m_imageLayers.clear();
    delete m_decorations;
    delete m_fillers;
    delete m_ground;
    delete m_floorsCollisionLayer;
    delete m_wallsCollisionLayer;
    delete m_playerSpawnsLayer;
    delete m_enemySpawnsLayer;
    delete m_enemyPatrolAreasLayer;
    delete m_map;
    m_decorations = nullptr;
    m_fillers = nullptr;
    m_ground = nullptr;
    m_floorsCollisionLayer = nullptr;
    m_wallsCollisionLayer = nullptr;
    m_playerSpawnsLayer = nullptr;
    m_enemySpawnsLayer = nullptr;
    m_enemyPatrolAreasLayer = nullptr;
    m_map = nullptr;
}

void Level::update(uint32_t deltaMilliseconds)
{
    // To-Do
}

void Level::render(sf::RenderWindow& window)
{
    for (const auto& imageLayer : m_imageLayers)
    {
        sf::RenderStates states;
        states.texture = imageLayer.second;
        window.draw(imageLayer.first, states);
    }
    if (m_decorations)
        window.draw(*m_decorations);
    if (m_fillers)
        window.draw(*m_fillers);
    if (m_ground)
        window.draw(*m_ground);
    //if (m_floorsCollisionLayer)
    //    window.draw(*m_floorsCollisionLayer);
    //if (m_wallsCollisionLayer)
    //    window.draw(*m_wallsCollisionLayer);
    if (m_playerSpawnsLayer)
        window.draw(*m_playerSpawnsLayer);
    if (m_enemySpawnsLayer)
        window.draw(*m_enemySpawnsLayer);
    if (m_enemyPatrolAreasLayer)
        window.draw(*m_enemyPatrolAreasLayer);
}
const std::vector<sf::Shape*>& Level::getFloorsCollisionShapes() const
{
    return m_floorsCollisionLayer->getShapes();
}

const std::vector<sf::Shape*>& Level::getWallsCollisionShapes() const
{
    return m_wallsCollisionLayer->getShapes();
}

const std::vector<sf::Shape*>& Level::getEnemyPatrolAreasShapes() const
{
    return m_enemyPatrolAreasLayer->getShapes();
}

const std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>>& Level::getPlayerSpawnPoints() const
{
    return m_playerSpawnsLayer->getPoints();
}

const std::vector<std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>>>& Level::getEnemySpawnPoints() const
{
    return m_enemySpawnsLayer->getPoints();
}

std::pair<sf::Vector2f, std::unordered_map<std::string, std::string>> Level::getPlayerSpawnPoint() const
{
    if (!m_playerSpawnsLayer || m_playerSpawnsLayer->getPoints().empty())
    {
        printf("Warning: No player spawn points found!\n");
        return std::make_pair(sf::Vector2f(94.f, 490.f), std::unordered_map<std::string, std::string>());
    }

    return m_playerSpawnsLayer->getPoints().front();
}

