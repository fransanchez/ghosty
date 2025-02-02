#include <Render/MapLayer.h>

MapLayer::MapLayer(const tmx::Map& map, std::size_t idx)
{
    const auto& layers = map.getLayers();
    if (map.getOrientation() != tmx::Orientation::Orthogonal)
    {
        std::cout << "Map is not orthogonal - nothing will be drawn" << std::endl;
    }
    else if (idx >= layers.size())
    {
        std::cout << "Layer index " << idx << " is out of range, layer count is " << layers.size() << std::endl;
    }
    else if (layers[idx]->getType() != tmx::Layer::Type::Tile)
    {
        std::cout << "layer " << idx << " is not a Tile layer..." << std::endl;
    }

    else
    {
        //round the chunk size to the nearest tile
        const auto tileSize = map.getTileSize();
        m_chunkSize.x = std::floor(m_chunkSize.x / tileSize.x) * tileSize.x;
        m_chunkSize.y = std::floor(m_chunkSize.y / tileSize.y) * tileSize.y;
        m_mapTileSize.x = map.getTileSize().x;
        m_mapTileSize.y = map.getTileSize().y;
        const auto& layer = layers[idx]->getLayerAs<tmx::TileLayer>();
        createChunks(map, layer);

        auto mapSize = map.getBounds();
        m_globalBounds.width = mapSize.width;
        m_globalBounds.height = mapSize.height;
    }
}

void MapLayer::setTile(std::int32_t tileX, std::int32_t tileY, tmx::TileLayer::Tile tile, bool refresh)
{
    sf::Vector2u chunkLocale;
    const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    selectedChunk->setTile(chunkLocale.x, chunkLocale.y, tile, refresh);
}

tmx::TileLayer::Tile MapLayer::getTile(std::int32_t tileX, std::int32_t tileY)
{
    sf::Vector2u chunkLocale;
    const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    return selectedChunk->getTile(chunkLocale.x, chunkLocale.y);
}
void MapLayer::setColor(std::int32_t tileX, std::int32_t tileY, sf::Color color, bool refresh)
{
    sf::Vector2u chunkLocale;
    const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    selectedChunk->setColor(chunkLocale.x, chunkLocale.y, color, refresh);
}

sf::Color MapLayer::getColor(std::int32_t tileX, std::int32_t tileY)
{
    sf::Vector2u chunkLocale;
    const auto& selectedChunk = getChunkAndTransform(tileX, tileY, chunkLocale);
    return selectedChunk->getColor(chunkLocale.x, chunkLocale.y);
}

void MapLayer::update(sf::Time elapsed)
{
    for (auto& c : m_visibleChunks)
    {
        for (AnimationState& as : c->getActiveAnimations())
        {
            as.currentTime += elapsed;

            tmx::TileLayer::Tile tile;
            std::int32_t animTime = 0;
            auto x = as.animTile.animation.frames.begin();
            while (animTime < as.currentTime.asMilliseconds())
            {
                if (x == as.animTile.animation.frames.end())
                {
                    x = as.animTile.animation.frames.begin();
                    as.currentTime -= sf::milliseconds(animTime);
                    animTime = 0;
                }

                tile.ID = x->tileID;
                animTime += x->duration;
                x++;
            }

            setTile(as.tileCords.x, as.tileCords.y, tile);
        }
    }
}

Chunk::Ptr& MapLayer::getChunkAndTransform(std::int32_t x, std::int32_t y, sf::Vector2u& chunkRelative)
{
    std::uint32_t chunkX = (x * m_mapTileSize.x) / static_cast<std::uint32_t>(m_chunkSize.x);
    std::uint32_t chunkY = (y * m_mapTileSize.y) / static_cast<std::uint32_t>(m_chunkSize.y);
    chunkRelative.x = ((x * m_mapTileSize.x) - chunkX * static_cast<std::uint32_t>(m_chunkSize.x)) / m_mapTileSize.x;
    chunkRelative.y = ((y * m_mapTileSize.y) - chunkY * static_cast<std::uint32_t>(m_chunkSize.y)) / m_mapTileSize.y;
    return  m_chunks[chunkX + chunkY * m_chunkCount.x];
}

void MapLayer::createChunks(const tmx::Map& map, const tmx::TileLayer& layer)
{
    //look up all the tile sets and load the textures
    const auto& tileSets = map.getTilesets();
    const auto& layerIDs = layer.getTiles();
    std::uint32_t maxID = std::numeric_limits<std::uint32_t>::max();
    std::vector<const tmx::Tileset*> usedTileSets;

    for (auto i = tileSets.rbegin(); i != tileSets.rend(); ++i)
    {
        for (const auto& tile : layerIDs)
        {
            if (tile.ID >= i->getFirstGID() && tile.ID < maxID)
            {
                usedTileSets.push_back(&(*i));
                break;
            }
        }
        maxID = i->getFirstGID();
    }

    sf::Image fallback;
    fallback.create(2, 2, sf::Color::Magenta);
    for (const auto& ts : usedTileSets)
    {
        const auto& path = ts->getImagePath();

        std::unique_ptr<sf::Texture> newTexture = std::make_unique<sf::Texture>();
        sf::Image img;
        if (!img.loadFromFile(path))
        {
            newTexture->loadFromImage(fallback);
        }
        else
        {
            if (ts->hasTransparency())
            {
                auto transparency = ts->getTransparencyColour();
                img.createMaskFromColor({ transparency.r, transparency.g, transparency.b, transparency.a });
            }
            newTexture->loadFromImage(img);
        }
        m_textureResource.insert(std::make_pair(path, std::move(newTexture)));
    }

    //calculate the number of chunks in the layer
    //and create each one
    const auto bounds = map.getBounds();
    m_chunkCount.x = static_cast<sf::Uint32>(std::ceil(bounds.width / m_chunkSize.x));
    m_chunkCount.y = static_cast<sf::Uint32>(std::ceil(bounds.height / m_chunkSize.y));

    sf::Vector2u tileSize(map.getTileSize().x, map.getTileSize().y);

    for (auto y = 0u; y < m_chunkCount.y; ++y)
    {
        sf::Vector2f tileCount(m_chunkSize.x / tileSize.x, m_chunkSize.y / tileSize.y);
        for (auto x = 0u; x < m_chunkCount.x; ++x)
        {
            // calculate size of each Chunk (clip against map)
            if ((x + 1) * m_chunkSize.x > bounds.width)
            {
                tileCount.x = (bounds.width - x * m_chunkSize.x) / map.getTileSize().x;
            }
            if ((y + 1) * m_chunkSize.y > bounds.height)
            {
                tileCount.y = (bounds.height - y * m_chunkSize.y) / map.getTileSize().y;
            }
            //m_chunks.emplace_back(std::make_unique<Chunk>(layer, usedTileSets,
            //    sf::Vector2f(x * m_chunkSize.x, y * m_chunkSize.y), tileCount, map.getTileCount().x, m_textureResource));
            m_chunks.emplace_back(std::make_unique<Chunk>(layer, usedTileSets,
                sf::Vector2f(x * m_chunkSize.x, y * m_chunkSize.y), tileCount, tileSize, map.getTileCount().x, m_textureResource, map.getAnimatedTiles()));
        }
    }
}

void MapLayer::updateVisibility(const sf::View& view) const
{
    sf::Vector2f viewCorner = view.getCenter();
    viewCorner -= view.getSize() / 2.f;

    std::int32_t posX = static_cast<std::int32_t>(std::floor(viewCorner.x / m_chunkSize.x));
    std::int32_t posY = static_cast<std::int32_t>(std::floor(viewCorner.y / m_chunkSize.y));
    std::int32_t posX2 = static_cast<std::int32_t>(std::ceil((viewCorner.x + view.getSize().x) / m_chunkSize.x));
    std::int32_t posY2 = static_cast<std::int32_t>(std::ceil((viewCorner.y + view.getSize().x) / m_chunkSize.y));

    std::vector<Chunk*> visible;
    for (auto y = posY; y < posY2; ++y)
    {
        for (auto x = posX; x < posX2; ++x)
        {
            std::size_t idx = y * std::int32_t(m_chunkCount.x) + x;
            if (idx >= 0u && idx < m_chunks.size() && !m_chunks[idx]->empty())
            {
                visible.push_back(m_chunks[idx].get());
            }
        }
    }

    std::swap(m_visibleChunks, visible);
}

void MapLayer::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
    states.transform.translate(m_offset);

    //calc view coverage and draw nearest chunks
    updateVisibility(rt.getView());
    for (const auto& c : m_visibleChunks)
    {
        rt.draw(*c, states);
    }
}