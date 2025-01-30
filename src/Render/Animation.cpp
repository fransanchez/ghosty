#include <Render/Animation.h>

Animation::Animation(const Animation& baseAnimation)
{
    // Copy from the BaseAnimation
    m_texture = baseAnimation.m_texture;
    m_frames = baseAnimation.m_frames;
    m_frameDuration = baseAnimation.m_frameDuration;
    m_loop = baseAnimation.m_loop;

    // Reset Animation
    reset();
}

Animation::~Animation()
{
    m_texture = nullptr;
    m_frames.clear();
}

void Animation::applyToSprite(sf::Sprite& sprite)
{
    if (!m_frames.empty())
    {
        sprite.setTexture(*m_texture);
        sprite.setTextureRect(m_frames[m_currentFrame]);
    }
}

void Animation::addFrame(sf::Texture* texture, sf::IntRect frameRect)
{
    m_texture = texture;
    m_frames.push_back(frameRect);
}

void Animation::update(float deltaMilliseconds)
{
    if (m_frames.empty())
        return;

    m_elapsedTime += deltaMilliseconds;

    while (m_elapsedTime >= m_frameDuration)
    {
        m_elapsedTime -= m_frameDuration;
        if (m_loop)
        {
            m_currentFrame = (m_currentFrame + 1) % m_frames.size();
        }
        else
        {
            m_currentFrame = std::min(m_currentFrame + 1, m_frames.size() - 1);
        }
    }
}

void Animation::setFrameDuration(float duration)
{
    m_frameDuration = duration;
}

void Animation::setLoop(bool loop)
{
    m_loop = loop;
}

void Animation::reset()
{
    m_currentFrame = 0;
    m_elapsedTime = 0.f;
}

bool Animation::isFinished() const
{
    return !m_loop && m_currentFrame == m_frames.size() - 1;
}
