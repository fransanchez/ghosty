#pragma once

#include <Gameplay/GameObject.h>
#include <Render/Animation.h>
#include <Render/AnimationType.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include <string>

class Player : public GameObject
{
public:
    struct PlayerDescriptor
    {
        sf::Vector2f position;
        sf::Vector2f speed{ .0f, .0f };
    };

    ~Player() override = default;

    bool init(const PlayerDescriptor& descriptor, const std::unordered_map<AnimationType, Animation>& animations);

    void setAnimation(AnimationType animationType);


    sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

    void update(float deltaMilliseconds) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Sprite m_sprite;
    sf::Vector2f m_direction{ .0f, .0f };
    sf::Vector2f m_speed{ 0.f, 0.f };

    std::unordered_map<AnimationType, Animation> m_animations;
    Animation* m_currentAnimation{ nullptr };

    void handleInput();
    void updateAnimation();
};
