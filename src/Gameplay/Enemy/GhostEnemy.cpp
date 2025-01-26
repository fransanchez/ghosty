#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/CollisionManager.h>
#include <Gameplay/Enemy/GhostEnemy.h>
#include <cmath>

bool GhostEnemy::isPlayerInRange() {
    return m_collisionManager->isPlayerInsideArea(m_collider->getBounds());
}
