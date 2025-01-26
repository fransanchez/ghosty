#pragma once

// Game constants
constexpr char* GAME_TITLE = "Ghosty";

// Config Constants 
constexpr char* PLAYER_CONFIG_PATH = "../data/Config/player_config_mage.json";
constexpr char* GHOST_ENEMY_CONFIG_PATH = "../data/Config/enemy_config_ghost.json";
constexpr char* SKELETON_ENEMY_CONFIG_PATH = "../data/Config/enemy_config_skeleton.json";
constexpr char* LEVEL_MAP_PATH = "../Data/Levels/Level1/Level1.tmx";

// Player Constants
constexpr float JUMP_INITIAL_VELOCITY = -500.0f;
constexpr float WALK_VELOCITY = 1.0f;
constexpr float RUN_VELOCITY = 1.5f;
constexpr float GRAVITY = 981.0f;

// Enemy Constants
constexpr float WALKING_ENEMY_IDLE_DURATION = 2000.0f;
constexpr float WALKING_ENEMY_PATROL_DURATION = 5000.0f;
constexpr float FLYING_ENEMY_OSCILLATION_RANGE = 0.7f;
constexpr float FLYING_ENEMY_OSCILLATION_SPEED = 2.f;
constexpr float FLYING_ENEMY_RETURN_TOLERANCE = 2.0f;