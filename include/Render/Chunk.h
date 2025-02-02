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

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/detail/Log.hpp>
#include <tmxlite/TileLayer.hpp>

struct AnimationState
{
    sf::Vector2u tileCords;
    sf::Time startTime;
    sf::Time currentTime;
    tmx::Tileset::Tile animTile;
    std::uint8_t flipFlags;
};

using TextureResource = std::map<std::string, std::unique_ptr<sf::Texture>>;

class Chunk final : public sf::Transformable, public sf::Drawable
{
public:
    using Ptr = std::unique_ptr<Chunk>;
    using Tile = std::array<sf::Vertex, 6u>;

    Chunk(const tmx::TileLayer& layer, std::vector<const tmx::Tileset*> tilesets,
        const sf::Vector2f& position, const sf::Vector2f& tileCount,
        const sf::Vector2u& tileSize, std::size_t rowSize,
        TextureResource& tr, const std::map<std::uint32_t, tmx::Tileset::Tile>& animTiles);

    void generateTiles(bool registerAnimation = false);
    ~Chunk() = default;
    Chunk(const Chunk&) = delete;
    Chunk& operator = (const Chunk&) = delete;

    std::vector<AnimationState>& getActiveAnimations() { return m_activeAnimations; }

    tmx::TileLayer::Tile getTile(std::int32_t x, std::int32_t y) const { return m_chunkTileIDs[calcIndexFrom(x, y)]; }

    sf::Color getColor(std::int32_t x, std::int32_t y) const { return m_chunkColors[calcIndexFrom(x, y)]; }

    bool empty() const { return m_chunkArrays.empty(); }

    void setTile(std::int32_t x, std::int32_t y, tmx::TileLayer::Tile tile, bool refresh);

    void setColor(std::int32_t x, std::int32_t y, sf::Color color, bool refresh);

    void maybeRegenerate(bool refresh);

    std::int32_t calcIndexFrom(std::int32_t x, std::int32_t y) const;

    void flipY(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3, sf::Vector2f* v4, sf::Vector2f* v5);

    void flipX(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3, sf::Vector2f* v4, sf::Vector2f* v5);

    void flipD(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3, sf::Vector2f* v4, sf::Vector2f* v5);

    void doFlips(std::uint8_t bits, sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3, sf::Vector2f* v4, sf::Vector2f* v5);

private:
    class ChunkArray final : public sf::Drawable
    {
    public:
        using Ptr = std::unique_ptr<ChunkArray>;

        tmx::Vector2u tileSetSize;
        sf::Vector2u tsTileCount;
        std::uint32_t m_firstGID, m_lastGID;

        explicit ChunkArray(const sf::Texture& t, const tmx::Tileset& ts);

        ~ChunkArray() = default;
        ChunkArray(const ChunkArray&) = delete;
        ChunkArray& operator = (const ChunkArray&) = delete;
        void reset();
        void addTile(const Chunk::Tile& tile);
        sf::Vector2u getTextureSize() const;

    private:
        const sf::Texture& m_texture;
        std::vector<sf::Vertex> m_vertices;
        void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

    };

    sf::Uint8 layerOpacity;     // opacity of the layer
    sf::Vector2f layerOffset;   // Layer offset
    sf::Vector2u mapTileSize;   // general Tilesize of Map
    sf::Vector2f chunkTileCount;   // chunk tilecount
    std::vector<tmx::TileLayer::Tile> m_chunkTileIDs; // stores all tiles in this chunk for later manipulation
    std::vector<sf::Color> m_chunkColors; // stores colors for extended color effects
    std::map<std::uint32_t, tmx::Tileset::Tile> m_animTiles;    // animation catalogue
    std::vector<AnimationState> m_activeAnimations;     // Animations to be done in this chunk
    std::vector<ChunkArray::Ptr> m_chunkArrays;

    void draw(sf::RenderTarget& rt, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        for (const auto& a : m_chunkArrays)
        {
            rt.draw(*a, states);
        }
    }
};
