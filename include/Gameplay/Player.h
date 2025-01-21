#pragma once

#include <Gameplay/Attack.h>
#include <Gameplay/GameObject.h>
#include <Gameplay/ProjectileAttack.h>
#include <memory>
#include <Render/Animation.h>
#include <Render/AnimationType.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <unordered_map>

class Player : public GameObject
{
    public:
        struct PlayerDescriptor
        {
            sf::Vector2f position;
            sf::Vector2f speed{ .0f, .0f };
        };

        ~Player() override = default;

        bool init(const PlayerDescriptor& descriptor,
            const std::unordered_map<AnimationType, Animation>& animations);

        void setAnimation(bool isRunning);

        void resetVerticalVelocity();

        sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }
        void setGrounded(bool grounded) { m_isGrounded = grounded; }
        bool isGrounded() const { return m_isGrounded; }

        void update(float deltaMilliseconds) override;
        void render(sf::RenderWindow& window) override;

    private:
        const float JUMP_INITIAL_VELOCITY = -500.0f;

        sf::Sprite m_sprite;
        sf::Vector2f m_direction{ .0f, .0f };
        sf::Vector2f m_speed{ 0.f, 0.f };
        bool m_isGrounded{ false };

        std::unordered_map<AnimationType, Animation> m_animations;
        Animation* m_currentAnimation{ nullptr };

        float m_gravity{ 981.0f };
        float m_verticalVelocity{ 0.0f };

        bool m_isAttacking{ false };

        void handleInput();
        void updateAnimation();
};
