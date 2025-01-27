#include <Gameplay/AttackSystem/Attack.h>
#include <Gameplay/CollisionManager.h>
#include <Gameplay/Enemy/VampireEnemy.h>

bool VampireEnemy::isPlayerInRange() {
    sf::Vector2f playerPosition = m_collisionManager->getPlayerPosition();
    float attackRange = m_attacks[m_currentAttackIndex]->getRange();
    if (std::abs(playerPosition.x - m_position.x) <= attackRange) {
        return true;
    }
    return false;
}