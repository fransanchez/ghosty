#pragma once

#include <Utils/Constants.h>
#include <Gameplay/Entity.h>
#include <Gameplay/EntityLife.h>
#include <memory>
#include <Render/Animation.h>
#include <Render/AnimationType.h>
#include <string>
#include <unordered_map>

class Attack;
class Collider;

class Player : public Entity
{
    public:
        struct PlayerDescriptor
        {
            sf::Vector2f position;
            sf::Vector2f speed{ .0f, .0f };
            std::unordered_map<AnimationType, Animation*>* animations;
            std::vector<Attack*> attacks;
            int maxLife = 3;
        };

        ~Player() override;

        bool init(const PlayerDescriptor& descriptor,
            Collider* collider,
            CollisionManager* collisionManager);

        void updateAnimationType();

        sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }
        void setGrounded(bool grounded);
        bool isGrounded() const { return m_isGrounded; }

        void update(float deltaMilliseconds) override; // From GameObject
        void handleCollisions() override; // From Collisionable

    private:
        void updatePlayerPosition(float deltaSeconds);
        void handleInput();
        void handleScenarioCollisions();
        void handleHurtingCollisions();
        void handleCollectibleCollisions();

        bool m_isGrounded{ false };
        bool m_isRunning{ false };
        float m_verticalVelocity{ 0.0f };
        bool m_attackKeyPressed{ false };
};
