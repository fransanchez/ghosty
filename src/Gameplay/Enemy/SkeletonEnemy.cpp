#include <Gameplay/Collisions/CollisionManager.h>
#include <Gameplay/Enemy/SkeletonEnemy.h>

bool SkeletonEnemy::isPlayerInRange() {
    return m_collisionManager->isPlayerInsideArea(m_collider->getBounds());
}

SoundType SkeletonEnemy::getHurtSoundType()
{
    return SoundType::SkeletonHurt;
}