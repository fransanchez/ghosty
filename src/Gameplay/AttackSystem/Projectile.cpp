#include <Gameplay/AttackSystem/Projectile.h>
#include <Gameplay/Collisions/CollisionManager.h>

void Projectile::init(ProjectileDescriptor descriptor,
            const Animation* anim,
            Collider* collider,
            CollisionManager* collisionManager)
{
    m_markedForDestruction = false;
    m_position = descriptor.position;
    m_direction = descriptor.direction;
    m_projectileSpeed = descriptor.projectileSpeed;
    m_lifetime = descriptor.projectileLife;
    m_damage = descriptor.damage;

    if (!m_animation)
    {
        m_animation = new Animation(*anim);
    }
    else
    {
        m_animation->reset();
    }
    m_collider = collider;
    m_collisionManager = collisionManager;
    m_collider->setPosition(m_position);
    m_animation->applyToSprite(m_sprite);
    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);
}

Projectile::~Projectile()
{
    delete m_animation;
    m_animation = nullptr;
}

sf::Vector2f Projectile::getPosition() const
{
    return m_position;
}

const Animation* Projectile::getAnimation() const
{
    return m_animation;
}

void Projectile::update(float deltaMilliseconds)
{
    if (m_markedForDestruction) {
        return;
    }
    handleCollisions();

    float deltaSeconds = deltaMilliseconds / 1000.f;
    m_lifetime -= deltaSeconds;

    m_position += m_direction * m_projectileSpeed * deltaSeconds;
    if (m_direction.x > 0.f) {
        m_sprite.setScale(1.0f, 1.0f);
    }
    else {
        m_sprite.setScale(-1.0f, 1.0f);
    }

    m_animation->update(deltaMilliseconds);
    m_animation->applyToSprite(m_sprite);
    m_sprite.setPosition(m_position);

    // Update collider
    Collisionable::update(deltaMilliseconds);
}

bool Projectile::isExpired() const
{
    return m_lifetime <= 0.0f;
}

void Projectile::markForDestruction()
{
    m_markedForDestruction = true;
}

bool Projectile::isMarkedForDestruction() const
{
    return m_markedForDestruction;
}

int Projectile::getDamage() const
{
    return m_damage;
}

void Projectile::render(sf::RenderWindow& window)
{
    window.draw(m_sprite);

    Collisionable::render(window);
}

void Projectile::handleCollisions()
{
    if (!m_collider || !m_collisionManager)
        return;

    auto wallCollision = m_collisionManager->checkWalls(m_collider);

    if (wallCollision.collided)
    {
        m_direction.x = 0.f;
        m_markedForDestruction = true;
    }
}