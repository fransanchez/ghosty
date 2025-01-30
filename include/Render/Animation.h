#pragma once

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Animation
{
public:

    Animation() = default;
    Animation(const Animation& baseAnimation);
    ~Animation();

    void addFrame(sf::Texture* texture, sf::IntRect frameRect);
    void Animation::applyToSprite(sf::Sprite& sprite);

    void update(float deltaMilliseconds);
    void setFrameDuration(float duration);
    void setLoop(bool loop);
    void reset();
    bool isFinished() const;

    const std::vector<sf::IntRect>& getFrames() const
    {
        return m_frames;
    }
    float getFrameDuration() const
    {
        return m_frameDuration;
    }

private:
    std::vector<sf::IntRect> m_frames;
    sf::Texture* m_texture;
    float m_frameDuration{ 0.1f };
    float m_elapsedTime{ 0.f };
    size_t m_currentFrame{ 0 };
    bool m_loop{ true };
};
