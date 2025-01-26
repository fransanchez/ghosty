#include <Gameplay/CollisionManager.h>
#include <Gameplay/Enemy/SkeletonEnemy.h>

bool SkeletonEnemy::isPlayerInRange() {
    return m_collisionManager->isPlayerInsideArea(m_collider->getBounds());
}