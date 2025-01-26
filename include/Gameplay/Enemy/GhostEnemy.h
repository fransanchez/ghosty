#pragma once

#include <Gameplay/Enemy/Enemy.h>

class GhostEnemy : public Enemy
{
public:
    GhostEnemy() = default;
    ~GhostEnemy() override = default;

    void GhostEnemy::update(float deltaMilliseconds) override;
private:
    void handleIdleState() override;
    void handlePatrolState() override;
    void handleChaseState() override;
    void handleTargetLockedState() override;
    void handleReturnToOriginState() override;
    void handleAttackState() override;
    bool isPlayerInRange() override;

    void applyOscillation();

    float m_oscillationTimer = 0;
};