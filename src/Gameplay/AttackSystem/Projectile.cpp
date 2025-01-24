#include <Gameplay/AttackSystem/Projectile.h>
#include <Gameplay/CollisionManager.h>

void Projectile::init(ProjectileDescriptor descriptor,
            const Animation* anim,
            Collider* collider,
            CollisionManager* collisionManager)
{
    m_position = descriptor.position;
    m_direction = descriptor.direction;
    m_projectileSpeed = descriptor.projectileSpeed;
    m_lifetime = descriptor.projectileLife;
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
    m_sprite.setTexture(*m_animation->getCurrentFrame());
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

void Projectile::update(float deltaTime)
{
    handleCollisions();

    m_lifetime -= deltaTime;
    m_position += m_direction * m_projectileSpeed * deltaTime;
    m_animation->update(deltaTime);
    m_sprite.setTexture(*m_animation->getCurrentFrame());
    m_sprite.setPosition(m_position);
    m_collider->setPosition(m_position);
}

bool Projectile::isExpired() const
{
    return m_lifetime <= 0.0f;
}

void Projectile::render(sf::RenderWindow& window)
{
    window.draw(m_sprite);

    m_collider->render(window);
}

void Projectile::handleCollisions()
{
    if (!m_collider || !m_collisionManager)
        return;

    auto wallCollision = m_collisionManager->checkWalls(m_collider);

    if (wallCollision.collided)
    {
        m_direction.x = 0.f;
        m_lifetime = 0.0f;
    }
}