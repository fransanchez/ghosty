#pragma once

#include <Gameplay/Enemy/Enemy.h>

class GhostEnemy : public Enemy
{
public:
    GhostEnemy() = default;
    ~GhostEnemy() override = default;

    void update(float deltaMilliseconds) override; // From GameObject
    void handleCollisions() override; // From Collisionable


private:
    void handleState(float deltaMilliseconds) override; // From Enemy
    void updateSight() override; // From Enemy

    void updateEnemySprite(float deltaSeconds);
    void updateEnemyPosition(float deltaSeconds);
    void handleIdleState();
    void handlePatrolState();
    void handleChaseState();
    void handleAttackState();

    float m_stateTimer{ 0.0f }; // Time spent in the current state
    const float IDLE_DURATION = 2000.0f; // Time to stay idle before patrolling (ms)
    const float PATROL_DURATION = 5000.0f;
    bool m_movingRight{ false };
};