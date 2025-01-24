#pragma once

#include <Utils/Constants.h>
#include <Gameplay/Collisionable.h>
#include <memory>
#include <Render/Animation.h>
#include <Render/AnimationType.h>
#include <string>
#include <unordered_map>

class Attack;
class Collider;

class Player : public Collisionable
{
    public:
        struct PlayerDescriptor
        {
            sf::Vector2f position;
            sf::Vector2f speed{ .0f, .0f };
        };

        ~Player() override;

        bool init(const PlayerDescriptor& descriptor,
            std::unordered_map<AnimationType, Animation*>& animations,
            std::vector<Attack*> attacks,
            Collider* collider,
            CollisionManager* collisionManager);

        void setAnimation(bool isRunning);

        sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }
        void setGrounded(bool grounded);
        bool isGrounded() const { return m_isGrounded; }

        sf::FloatRect getSpriteBounds() const;

        void update(float deltaMilliseconds) override; // From GameObject
        void render(sf::RenderWindow& window) override; // From GameObject
        void handleCollisions() override; // From Collisionable

    private:

        std::vector<Attack*> m_attacks;

        sf::Sprite m_sprite;
        sf::Vector2f m_direction{ .0f, .0f };
        sf::Vector2f m_speed{ 0.f, 0.f };
        bool m_isGrounded{ false };

        std::unordered_map<AnimationType, Animation*> m_animations;
        Animation* m_currentAnimation{ nullptr };

        int m_currentAttackIndex{ 1 };

        float m_gravity{ GRAVITY };
        float m_verticalVelocity{ 0.0f };

        bool m_isAttacking{ false };
        bool m_attackKeyPressed{ false };

        void updatePlayerPosition(float deltaSeconds);
        void updateSpriteSelection(float deltaSeconds);
        void handleInput();
        void updateAnimation();
};
