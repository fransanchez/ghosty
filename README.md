# Ghosty - Platformer Game
Ghosty is a platform-style game inspired by Ghosts 'n Goblins, where you control Ghosty, the main character, using ranged spells to defeat monsters and survive through different levels filled with enemies, collectibles, and obstacles.

## How to Build and Run
### Prerequisites
1. Install [Git](https://git-scm.com/downloads) and [CMake](https://cmake.org/download/). Use your system's package manager if available.

### Steps to Build

1. Clone the Repository: git clone https://github.com/fransanchez/ghosty.git
2. Call CMake from the root directory:  
   cmake -B build
   cmake --build ../build
3. (Optional) If you are on Windows, go to scripts folder and run build.bat.
4. (Optional) If you want to use Visual Studio 2022, run generate.bat from the scripts folder to generarte the solution

## Features

- Character: Play as Ghosty, a character equipped with a variety of magical ranged attacks.
- Enemies: Fight 4 types of enemies: Dino, Ghost, Skeleton, and Vampire.
- Collectibles: Includes power-ups like extra lives and magic enhancements (Fire, Ice, Wind).
- AI: Enemies have basic AI with patrolling, chasing, and attacking behaviors.
- Collision Detection: Integrated collision management for characters, enemies, and projectiles.

## Game Controls

- A/D: Move left / right
- W / Space: Jump
- F: Attack with magic spells
- LShift: Sprint
- Escape: Exit the game

## Power ups and lives

- Temporary power-ups last 15 seconds and enhance your magic abilities with more damage and range.
- You have 5 lives. If you lose all of them or fall into water, the game ends.

## Game Architecture

### Modules Overview

#### Core System
- AssetManager: Manages loading of game assets like textures and sounds.
- AudioManager: Handles background music and sound effects during gameplay.
- Game: Controls the main game loop and coordinates between the different subsystems.
- World: Contains the game world, managing entities and interactions.

#### Gameplay
- AttackSystem: Manages attack types like melee and ranged attacks.
- Collectibles: Manages collectible items such as extra lives and magic power-ups.
- Collisions: Handles collision detection for all game entities.
- Enemies: Defines the behavior of different types of enemies (Flying, Walking, Static).
- Player: The player entity, including movement, health, and attacks.
- Entity: Base class for all game entities (Player, Enemies), managing attributes and behavior.

#### Rendering
- Animation: Manages sprite animations for characters and enemies.
- Chunk: Handles map tiles and manages the rendering of large levels.
- MapLayer: Organizes and renders different layers of the map (e.g., floors, background).
- ObjectLayer: Organizes and renders different layers with objects of the map (e.g., enemy spawns, collectibles, patrol areas).

#### UI
- UIManager: Controls the game's UI, including HUD and menus.
- HUD: Displays health, magic power-ups, and other player information.
- UIScreen: Handles various screens like the main menu, in-game UI, and game-over screens.

#### Utils
- GameConfigLoader: Loads game settings and configurations from JSON files.
- ObjectPool: Manages the pooling of reusable objects, like projectiles and enemies.


## Technical Decisions

1. Pointer Management (Raw Pointers): Raw pointers were used instead of smart pointers for better control over memory management. This approach helped with managing object lifecycles and dependencies.

2. Map and Layers: The game uses an efficient map system where each layer is loaded separately, including enemy spawn points and collectible locations. This system allows easy extension by adding new entities or items without modifying the code.

3. Enemy Management: The EnemyManager uses pooling to manage enemies efficiently. Each type of enemy has its own pool, and only enemies within the camera view are loaded to optimize performance.

4. Entity System: The Entity class is the backbone for all game entities (Player and Enemy for now). It manages health, animations, and interactions. AttackSystem allows easy extension of attack types via JSON configuration files.

## Future Enhancements

- Improved Enemy AI: Simplify AI by implementing decision trees or finite state machines to make enemy behaviors more modular.

- Dynamic Asset Loading: Implement dynamic loading of sounds, textures, and other resources based on the active level or screen to save memory and improve performance.

- Map Rendering Optimization: Enhance performance by only rendering visible tiles when the camera moves, optimizing rendering in larger levels.

- UI Enhancements: Add additional information to the HUD, such as indicators for magic power-ups, health, and proximity of enemies.


## License
This project is licensed under the MIT License. See [LICENSE.md](https://github.com/fransanchez/ghosty/blob/main/LICENSE.md) for full details.