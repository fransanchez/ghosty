#include <Gameplay/Collisions/CollisionManager.h>
#include <Gameplay/Enemy/DinoEnemy.h>

bool DinoEnemy::isPlayerInRange() {
    return m_collisionManager->isPlayerInsideArea(m_collider->getBounds());
}