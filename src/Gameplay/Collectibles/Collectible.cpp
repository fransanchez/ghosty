#include <Core/AudioManager.h>
#include <Gameplay/Collectibles/Collectible.h>
#include <Gameplay/Player/Player.h>
#include <SFML/Graphics/RenderWindow.hpp>

bool Collectible::init(
    CollectibleType type,
    Animation* animation,
    CollisionManager* collisionManager,
    sf::Vector2f position)
{
    m_collisionManager = collisionManager;
    m_type = type;
    m_animation = animation;
    setPosition(position);
    m_sprite.setPosition(position);

    if (m_animation && !m_animation->getFrames().empty())
    {
        m_animation->applyToSprite(m_sprite);
        sf::FloatRect spriteBounds = m_sprite.getLocalBounds();
        m_sprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);
        m_collider = new Collider(position, { spriteBounds.width, spriteBounds.height }, { 0.f, 0.f });
    }
    else {
        printf("Error: Collectible Animation is missing\n");
        return false;
    }
    return true;
}

Collectible::~Collectible()
{
    delete m_animation;
    m_animation = nullptr;
}

void Collectible::update(float deltaMilliseconds)
{
    if (m_animation && !m_animation->getFrames().empty())
    {
        m_animation->update(deltaMilliseconds);
        m_animation->applyToSprite(m_sprite);
    }
}

void Collectible::render(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void Collectible::applyEffect(Player* player) 
{
    // To-Do make this better if I have time
    if (player) {
        switch (m_type)
        {
        case CollectibleType::Life:
            player->addLives(1);
            break;
        case CollectibleType::Fire:
            player->setTemporaryAttackIndex(1);
            break;
        case CollectibleType::Ice:
            player->setTemporaryAttackIndex(2);
            break;
        case CollectibleType::Wind:
            player->setTemporaryAttackIndex(3);
            break;
        }
    }
    m_markedForDestruction = true;
    AudioManager::getInstance()->playSoundEffect(SoundType::ItemPickup);
}

void Collectible::reset()
{
    delete m_animation;
    m_animation = nullptr;
    m_markedForDestruction = false;
    m_type = CollectibleType::None;
}

void Collectible::handleCollisions()
{
    // Collisions with this handled by the player
}


