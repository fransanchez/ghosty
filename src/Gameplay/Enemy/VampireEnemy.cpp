#include <Gameplay/CollisionManager.h>
#include <Gameplay/Enemy/VampireEnemy.h>

bool VampireEnemy::isPlayerInRange() {
    sf::Vector2f playerPosition = m_collisionManager->getPlayerPosition();
    if (std::abs(playerPosition.x - m_position.x) <= 500.f) {
        return true;
    }
    return false;
    //return m_collisionManager->isPlayerInsideArea(m_collider->getBounds());
}