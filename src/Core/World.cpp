#include <Core/World.h>
#include <Core/AssetManager.h>
#include <Gameplay/Enemy/Zombie.h>
#include <Gameplay/Player/PlayerFactory.h>

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
	//Zombie* zombie = new Zombie();

	Zombie& zombie = m_zombiesPool.get();
	const bool initOk = zombie.init(zombieDescriptor);

	m_enemy = &zombie;
	zombie.setPosition({ .0f, 50.f });


	m_player = PlayerFactory::createPlayer("../data/Config/player_config_mage.json", { 100.f, 100.f }, { 200.f, 150.f });
	if (!m_player)
	{
		return false;
	}

	const std::string mapFile = "../Data/Levels/test.tmx";

	m_level = new Level();
	if (!m_level->load(mapFile))
	{
		return false;
	}


	return true;
}

void World::unload()
{
	Zombie* z = dynamic_cast<Zombie*>(m_enemy);
	m_zombiesPool.release(*z);
	delete m_player;
	m_player = nullptr;
	m_level->unload();
	m_level = nullptr;
}

void World::update(uint32_t deltaMilliseconds)
{
	m_level->update(deltaMilliseconds);

	// Update actors
	m_player->update(deltaMilliseconds);

	m_enemy->update(deltaMilliseconds);

	handleCollisions();

}

void World::render(sf::RenderWindow& window)
{
	m_level->render(window);
	m_player->render(window);

	m_enemy->render(window);
}


void World::handleCollisions()
{
	const auto& collisionShapes = m_level->getCollisionShapes();
	sf::FloatRect playerBounds = m_player->getBounds();

	bool isGrounded = false;

	if (m_level->isGrounded(playerBounds))
	{
		isGrounded = true;
	}
	m_player->setGrounded(isGrounded);

}

