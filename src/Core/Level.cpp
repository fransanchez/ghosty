#include <Core/Level.h>
#include <tmxlite/Map.hpp>
#include <Render/SFMLOrthogonalLayer.h>
#include <SFML/Graphics/RenderWindow.hpp>

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

    m_layerZero = new MapLayer(*m_map, 0);
    m_layerOne = new MapLayer(*m_map, 1);
    m_layerTwo = new MapLayer(*m_map, 2);

    m_collisionLayer = new ObjectLayer(*m_map, 3);

    return true;
}

void Level::unload()
{
    delete m_collisionLayer;
    delete m_layerTwo;
    delete m_layerOne;
    delete m_layerZero;
    delete m_map;
    m_collisionLayer = nullptr;
    m_layerTwo = nullptr;
    m_layerOne = nullptr;
    m_layerZero = nullptr;
    m_map = nullptr;
}

void Level::update(uint32_t deltaMilliseconds)
{
    if (m_layerZero)
        m_layerZero->update(sf::milliseconds(deltaMilliseconds));
}

void Level::render(sf::RenderWindow& window)
{
    if (m_layerZero)
        window.draw(*m_layerZero);
    if (m_layerOne)
        window.draw(*m_layerOne);
    if (m_layerTwo)
        window.draw(*m_layerTwo);

    if (m_collisionLayer)
        window.draw(*m_collisionLayer);
}

const std::vector<sf::Shape*>& Level::getCollisionShapes() const
{
    return m_collisionLayer->getShapes();
}

bool Level::isGrounded(const sf::FloatRect& bounds) const
{
    for (const auto* shape : m_collisionLayer->getShapes())
    {
        sf::FloatRect groundBounds = shape->getGlobalBounds();

        const float margin = 4.f;
        sf::FloatRect playerBottom(bounds.left, bounds.top + bounds.height - 1.f, bounds.width, margin);
        sf::FloatRect groundTop(groundBounds.left, groundBounds.top, groundBounds.width, margin);

        if (playerBottom.intersects(groundTop))
        {
            return true;
        }
    }

    return false;
}
