#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/CollisionManager.h>
#include <Gameplay/Enemy/GhostEnemy.h>
#include <cmath>


constexpr float OSCILLATION_RANGE = 0.7f;
constexpr float OSCILLATION_SPEED = 2.f;
constexpr float RETURN_TOLERANCE = 2.0f;

bool GhostEnemy::isPlayerInRange() {
    return m_collisionManager->isPlayerInsideArea(m_collider->getBounds());
}
