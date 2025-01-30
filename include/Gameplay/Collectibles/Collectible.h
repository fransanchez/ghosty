#pragma once

#include <Gameplay/Collectibles/CollectibleType.h>
#include <Gameplay/Collisions/Collisionable.h>
#include <Render/Animation.h>
#include <SFML/Graphics/Sprite.hpp>

class Player;

class Collectible : public Collisionable
{
    public:
        Collectible() = default;
        ~Collectible() override;

        bool init(CollectibleType type, Animation* animation, CollisionManager* collisionManager, sf::Vector2f position);
        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;
        void handleCollisions() override;

        void applyEffect(Player* player);
        bool isMarkedForDestruction() const { return m_markedForDestruction; }
        void reset();

    protected:
        CollectibleType m_type;
        Animation* m_animation{ nullptr };
        sf::Sprite m_sprite;
        bool m_markedForDestruction{ false };
};
