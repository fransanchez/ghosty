#include <Render/Animation.h>

void Animation::addFrame(const sf::Texture* texture)
{
    m_frames.push_back(texture);
}

const sf::Texture* Animation::getCurrentFrame() const
{
    if (m_frames.empty())
        return nullptr;

    return m_frames[m_currentFrame];
}

void Animation::update(float deltaTime)
{
    if (m_frames.empty())
        return;

    m_elapsedTime += deltaTime;

    while (m_elapsedTime >= m_frameDuration)
    {
        m_elapsedTime -= m_frameDuration;
        m_currentFrame = (m_currentFrame + 1) % m_frames.size();
    }
}

void Animation::setFrameDuration(float duration)
{
    m_frameDuration = duration;
}

void Animation::reset()
{
    m_currentFrame = 0;
    m_elapsedTime = 0.f;
}

