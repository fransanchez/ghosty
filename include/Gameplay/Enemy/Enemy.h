#pragma once

#include <Gameplay/Collisionable.h>
#include <Render/Animation.h>
#include <Render/AnimationType.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Attack;
class Collider;

class Enemy : public Collisionable
{
	public:
		enum class EnemyState
		{
			Idle,
			Patrol,
			Chase,
			TargetLocked,
			ReturnToOrigin,
			Attack
		};

		struct EnemyDescriptor
		{
			sf::Vector2f position;
			sf::Vector2f speed{ .0f, .0f };
			sf::Vector2f sightArea;
			std::unordered_map<AnimationType, Animation*>* animations;
			std::vector<Attack*> attacks;
			const sf::Shape* patrolArea = nullptr;
		};

		virtual ~Enemy() override;

		bool init(const EnemyDescriptor& enemyDescriptor, Collider* collider, CollisionManager* collisionManager);

		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

		virtual void update(float deltaMilliseconds) override; // From GameObject
		virtual void render(sf::RenderWindow& window) override; // From GameObject
		virtual void handleCollisions() override; // From Collisionable

	protected:
		virtual void handleIdleState() = 0;
		virtual void handlePatrolState() = 0;
		virtual void handleChaseState() = 0;
		virtual void handleTargetLockedState() = 0;
		virtual void handleReturnToOriginState() = 0;
		virtual void handleAttackState() = 0;

		void changeState(EnemyState newState);
		void updateAnimation();
		void setSpeedForState();

		void updateEnemyPosition(float deltaSeconds);
		void updateEnemySprite(float deltaSeconds);
		void updateSight();
		void handleState(float deltaMilliseconds);
		void moveWithinAreaEdges();
		bool isPlayerInSight();
		bool isPlayerInArea();

		sf::Sprite m_sprite;
		sf::Vector2f m_speed{ 0.f, 0.f };
		sf::Vector2f m_patrolSpeed;
		sf::Vector2f m_chaseSpeed;
		sf::Vector2f m_originalPosition;
		std::unordered_map<AnimationType, Animation*>* m_animations { nullptr };
		std::vector<Attack*> m_attacks;

		Animation* m_currentAnimation{ nullptr };
		int m_currentAttackIndex{ 0 };
		EnemyState m_currentState{ EnemyState::Idle };

		sf::Vector2f m_sightArea{ 300.f, 150.f };
		sf::RectangleShape m_enemySight;
		bool m_movingRight{ false };

		const sf::Shape* m_patrolArea = nullptr;
};