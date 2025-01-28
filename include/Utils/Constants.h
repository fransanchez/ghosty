#pragma once

// Game constants
constexpr char* GAME_TITLE = "Ghosty";
constexpr char* GAME_CONFIG_PATH = "../data/Config/game_config.json";

// Config Constants
constexpr char* DINO_ENEMY_CONFIG_PATH = "../data/Config/enemy_config_dino.json";
constexpr char* GHOST_ENEMY_CONFIG_PATH = "../data/Config/enemy_config_ghost.json";
constexpr char* LEVEL_MAP_PATH = "../Data/Levels/Level1/Level1.tmx";
constexpr char* PLAYER_CONFIG_PATH = "../data/Config/player_config_mage.json";
constexpr char* SKELETON_ENEMY_CONFIG_PATH = "../data/Config/enemy_config_skeleton.json";
constexpr char* VAMPIRE_ENEMY_CONFIG_PATH = "../data/Config/enemy_config_vampire.json";

// Player Constants
constexpr float JUMP_INITIAL_VELOCITY = -500.0f;
constexpr float GRAVITY = 981.0f;
constexpr float RUN_VELOCITY = 1.5f;
constexpr float WALK_VELOCITY = 1.0f;

// Enemy Constants
constexpr float ENEMY_RETURN_POSITION_TOLERANCE = 2.0f;
constexpr float FLYING_ENEMY_OSCILLATION_RANGE = 0.7f;
constexpr float FLYING_ENEMY_OSCILLATION_SPEED = 2.f;
constexpr float STATIC_ENEMY_WATCH_DURATION = 5000.0f;
constexpr float WALKING_ENEMY_IDLE_DURATION = 2000.0f;
constexpr float WALKING_ENEMY_PATROL_DURATION = 5000.0f;

