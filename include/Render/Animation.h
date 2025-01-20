#pragma once

#include <vector>
#include <SFML/Graphics/Texture.hpp>

class Animation
{
public:
    void addFrame(const sf::Texture* texture);
    const sf::Texture* getCurrentFrame() const;

    void update(float deltaTime);
    void setFrameDuration(float duration);
    void reset();

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
};
