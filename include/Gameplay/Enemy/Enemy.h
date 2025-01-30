#pragma once

#include <Gameplay/Enemy/Enemy.h>
#include <Gameplay/Enemy/EnemyType.h>
#include <Gameplay/Entity.h>
#include <Gameplay/EntityLife.h>
#include <Render/Animation.h>
#include <Render/AnimationType.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Attack;
class Collider;

class Enemy : public Entity
{
	public:
		enum class EnemyState
		{
			Idle,
			Patrol,
			Chase,
			TargetLocked,
			ReturnToOrigin,
			Attack,
			Hurt,
			Dead
		};

		struct EnemyDescriptor
		{
			sf::Vector2f position;
			sf::Vector2f speed{ .0f, .0f };
			sf::Vector2f sightArea;
			std::unordered_map<AnimationType, Animation*>* animations;
			std::vector<Attack*> attacks;
			const sf::Shape* patrolArea = nullptr;
			int maxLife = 1;
			EnemyType type;
			Collider* collider = nullptr;
		};

		virtual ~Enemy() override;

		bool init(const EnemyDescriptor& enemyDescriptor, CollisionManager* collisionManager);

		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); };

		EnemyType getType() const { return m_type; }

		virtual void update(float deltaMilliseconds) override; // From GameObject
		virtual void render(sf::RenderWindow& window) override; // From GameObject
		void handleCollisions() override; // From Collisionable

		void reset() override;

	protected:
		virtual void handleIdleState() = 0;
		virtual void handlePatrolState() = 0;
		virtual void handleChaseState() = 0;
		virtual void handleTargetLockedState() = 0;
		virtual void handleReturnToOriginState() = 0;
		virtual void handleAttackState() = 0;

		void changeState(EnemyState newState);
		void updateAnimationType();
		void setSpeedForState();

		void updateSight();
		void handleState(float deltaMilliseconds);
		void moveWithinAreaEdges();
		bool isPlayerInSight();
		bool isPlayerInArea();

		sf::Vector2f m_patrolSpeed;
		sf::Vector2f m_chaseSpeed;
		sf::Vector2f m_originalPosition;

		EnemyState m_currentState{ EnemyState::Idle };

		sf::Vector2f m_sightArea{ 300.f, 150.f };
		sf::RectangleShape m_enemySight;

		const sf::Shape* m_patrolArea = nullptr;

		EnemyType m_type;
};