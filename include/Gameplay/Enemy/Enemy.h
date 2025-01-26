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
			Attack
		};

		struct EnemyDescriptor
		{
			sf::Vector2f position;
			sf::Vector2f speed{ .0f, .0f };
			float sightRange{ 50.f };
			std::unordered_map<AnimationType, Animation*>* animations;
			std::vector<Attack*> attacks;
		};

		virtual ~Enemy() override;

		bool init(const EnemyDescriptor& enemyDescriptor, Collider* collider, CollisionManager* collisionManager);

		sf::FloatRect getBounds() const { return m_sprite.getGlobalBounds(); }

		void render(sf::RenderWindow& window) override; // From GameObject

	protected:
		virtual void handleState(float deltaMilliseconds) = 0;
		void changeState(EnemyState newState);
		void updateAnimation();
		virtual void updateSight() = 0;

		sf::Sprite m_sprite;
		sf::Vector2f m_direction{ .0f, .0f };
		sf::Vector2f m_speed{ 0.f, 0.f };
		std::unordered_map<AnimationType, Animation*>* m_animations;
		std::vector<Attack*> m_attacks;

		Animation* m_currentAnimation{ nullptr };
		int m_currentAttackIndex{ 0 };
		EnemyState m_currentState{ EnemyState::Idle };

		sf::RectangleShape m_enemySight;
		float m_sightRange{ 50.f };
};