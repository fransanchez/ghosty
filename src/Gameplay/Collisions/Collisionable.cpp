#include <Gameplay/Collisions/Collisionable.h>
#include <Gameplay/Collisions/CollisionManager.h>

Collisionable::Collisionable(Collider* collider, CollisionManager* manager) {

    m_collider = collider;
    m_collisionManager = manager;
}

Collisionable::~Collisionable() {
    delete(m_collider);
    m_collider = nullptr;
    m_collisionManager = nullptr;
}

Collider* Collisionable::getCollider() const
{
    return m_collider;
}

CollisionManager* Collisionable::getCollisionManager() const
{
    return m_collisionManager;
}

void Collisionable::update(float deltaMilliseconds)
{
    m_collider->setPosition(getPosition());
    m_collider->setDirection(getDirection());
}
void Collisionable::render(sf::RenderWindow& window)
{
    m_collider->render(window);
}
