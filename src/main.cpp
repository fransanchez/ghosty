
#include <cstdio>
#include <Core/Game.h>
#include <nlohmann/json.hpp>
#include <SFML/System/Clock.hpp>
#include <Utils/Constants.h>
#include <Utils/GameConfigLoader.h>


int main()
{
    nlohmann::json config = GameConfigLoader::loadConfig(GAME_CONFIG_PATH);

    if (config.empty())
    {
        printf("Error: Failed to load config from file \n");
        return false;
    }

    Game::GameCreateInfo gameCI;
    gameCI.gameTitle = config.value("gameTitle", "SFML Game");
    gameCI.screenWidth = config.value("screenWidth", 1280u);
    gameCI.screenHeight = config.value("screenHeight", 720u);
    gameCI.frameRateLimit = config.value("frameRateLimit", 60u);
    gameCI.windowIconPath = config.value("windowIconPath", "");
    gameCI.windowStyle = config.value("windowStyle", "FullScreen");
    config.clear();

    Game game;
    const bool gameInitialized = game.init(gameCI);

    if (gameInitialized == false)
    {
        printf("Game could not be initialized\n");
        return 0;
    }

    sf::Clock clock;

    while (game.isRunning())
    {
        const sf::Time elapsed = clock.restart();

        game.update(elapsed.asMilliseconds());
        game.render();
    }
}
