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

    void addFrame(const sf::Texture* texture);
    const sf::Texture* getCurrentFrame() const;

    void update(float deltaTime);
    void setFrameDuration(float duration);
    void setLoop(bool loop);
    void reset();
    bool isFinished() const;

    const std::vector<const sf::Texture*>& getFrames() const
    {
        return m_frames;
    }
    float getFrameDuration() const
    {
        return m_frameDuration;
    }

private:
    std::vector<const sf::Texture*> m_frames;
    float m_frameDuration{ 0.1f };
    float m_elapsedTime{ 0.f };
    size_t m_currentFrame{ 0 };
    int counter = 0;
    bool m_loop{ true };
};
