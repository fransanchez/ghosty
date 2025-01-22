#include <Gameplay/AttackSystem/Projectile.h>

void Projectile::init(const sf::Vector2f& pos, const sf::Vector2f& dir, float projectileSpeed, float life, const Animation& anim)
{
    m_position = pos;
    m_direction = dir;
    m_projectileSpeed = projectileSpeed;
    m_lifetime = life;
    m_animation = anim;
}

Projectile::~Projectile()
{
    // Limpieza o depuración si es necesario
    printf("Projectile destroyed.\n");
}

sf::Vector2f Projectile::getPosition() const
{
    return m_position;
}

const Animation& Projectile::getAnimation() const
{
    return m_animation;
}

void Projectile::update(float deltaTime)
{
    m_lifetime -= deltaTime;
    m_position += m_direction * m_projectileSpeed * deltaTime;
    m_animation.update(deltaTime);
    m_sprite.setTexture(*m_animation.getCurrentFrame());
    m_sprite.setPosition(m_position);
}

bool Projectile::isExpired() const
{
    return m_lifetime <= 0.0f;
}

void Projectile::render(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}