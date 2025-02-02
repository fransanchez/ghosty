/*********************************************************************
(c) Matt Marchant & contributors 2016 - 2024
http://trederia.blogspot.com

tmxlite - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

/*
Creates an SFML drawable from an Orthogonal tmx map layer.
This is an example of drawing with SFML - not all features
are implemented.
*/

#pragma once

#include <Render/Chunk.h>
#include <tmxlite/TileLayer.hpp>

class MapLayer final : public sf::Drawable
{
public:

    MapLayer(const tmx::Map& map, std::size_t idx);
    ~MapLayer() = default;
    MapLayer(const MapLayer&) = delete;
    MapLayer& operator = (const MapLayer&) = delete;

    const sf::FloatRect& getGlobalBounds() const { return m_globalBounds; }
    void setOffset(sf::Vector2f offset) { m_offset = offset; }
    sf::Vector2f getOffset() const { return m_offset; }

    void setTile(std::int32_t tileX, std::int32_t tileY, tmx::TileLayer::Tile tile, bool refresh = true);

    tmx::TileLayer::Tile getTile(std::int32_t tileX, std::int32_t tileY);

    void setColor(std::int32_t tileX, std::int32_t tileY, sf::Color color, bool refresh = true);

    sf::Color getColor(std::int32_t tileX, std::int32_t tileY);

    void update(sf::Time elapsed);

private:
    //increasing m_chunkSize by 4; fixes render problems when mapsize != chunksize
    //sf::Vector2f m_chunkSize = sf::Vector2f(1024.f, 1024.f);
    sf::Vector2f m_chunkSize = sf::Vector2f(512.f, 512.f);
    sf::Vector2u m_chunkCount;
    sf::Vector2u m_mapTileSize;   // general Tilesize of Map
    sf::FloatRect m_globalBounds;
    sf::Vector2f m_offset;

    std::vector<Chunk::Ptr> m_chunks;
    mutable std::vector<Chunk*> m_visibleChunks;

    using TextureResource = std::map<std::string, std::unique_ptr<sf::Texture>>;
    TextureResource m_textureResource;

    Chunk::Ptr& getChunkAndTransform(std::int32_t x, std::int32_t y, sf::Vector2u& chunkRelative);

    void createChunks(const tmx::Map& map, const tmx::TileLayer& layer);

    void updateVisibility(const sf::View& view) const;

    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
};