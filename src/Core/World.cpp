#include <Core/World.h>
#include <Core/AssetManager.h>
#include <Gameplay/Zombie.h>

World::~World()
{
	unload();
}

bool World::load()
{
	constexpr float millisecondsToSeconds = 1 / 1000.f;

	// To-Do, read ALL from file, this is just a quick example to understand that here is where entities are created but consider grouping/managing actors in a smarter way
	sf::Texture* zombieTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Enemies/zombie.png");
	Zombie::ZombieDescriptor zombieDescriptor;
	zombieDescriptor.texture = zombieTexture;
	zombieDescriptor.position = { 50.f, 50.f };
	zombieDescriptor.speed = { 400.f * millisecondsToSeconds, .0f }; // 400 units per second, or 0.4 units per millisecond, using the latter so it's in alignment with delta time
	zombieDescriptor.tileWidth = 192.f;
	zombieDescriptor.tileHeight = 256.f;
	Zombie* zombie = new Zombie();
	const bool initOk = zombie->init(zombieDescriptor);

	m_enemy = zombie;
	zombie->setPosition({ .0f, 50.f });

	const std::string mapFile = "../Data/Levels/test.tmx";

	if (!m_level.load(mapFile))
	{
		return false;
	}

	return initOk;
}

void World::unload()
{
	delete m_enemy;
	m_enemy = nullptr;
	m_level.unload();
}

void World::update(uint32_t deltaMilliseconds)
{
	m_level.update(deltaMilliseconds);

	// Update actors
	m_enemy->update(deltaMilliseconds);

	// Check for collisions (We could do it in a function here or have a collision manager if it gets complex)
	const auto& collisionShapes = m_level.getCollisionShapes();
	for (const auto* shape : collisionShapes)
	{
		if (shape->getGlobalBounds().intersects(m_enemy->getBounds()))
		{
#if DEBUG_MODE
			printf("Collision is detected");
#endif
		}
	}
}

void World::render(sf::RenderWindow& window)
{
	m_level.render(window);
	m_enemy->render(window);
}