#pragma once

// Game constants
constexpr char* GAME_TITLE = "Ghosty";
constexpr char* GAME_CONFIG_PATH = "../data/Config/game_config.json";
constexpr char* TITLE_SCREEN_IMAGE_PATH = "../data/Images/UI/GhostyMainTitle.png";
constexpr char* START_BUTTON_IMAGE_PATH = "../data/Images/UI/StartGameButton.png";
constexpr char* GAME_OVER_SCREEN_IMAGE_PATH = "../data/Images/UI/GameOver.png";
constexpr char* VICTORY_SCREEN_IMAGE_PATH = "../data/Images/UI/Victory.png";

// Config Constants
constexpr char* DINO_ENEMY_CONFIG_PATH = "../data/Config/enemy_config_dino.json";
constexpr char* GHOST_ENEMY_CONFIG_PATH = "../data/Config/enemy_config_ghost.json";
constexpr char* LEVEL_MAP_PATH = "../data/Levels/Level1/Level1.tmx";
constexpr char* PLAYER_CONFIG_PATH = "../data/Config/player_config_mage.json";
constexpr char* SKELETON_ENEMY_CONFIG_PATH = "../data/Config/enemy_config_skeleton.json";
constexpr char* VAMPIRE_ENEMY_CONFIG_PATH = "../data/Config/enemy_config_vampire.json";
constexpr char* COLLECTIBLES_CONFIG_PATH = "../data/Config/collectibles_config.json";
constexpr char* AUDIO_CONFIG_PATH = "../data/Config/audio_config.json";

// Player Constants
constexpr float JUMP_INITIAL_VELOCITY = -500.0f;
constexpr float GRAVITY = 981.0f;
constexpr float RUN_VELOCITY = 1.5f;
constexpr float WALK_VELOCITY = 1.0f;
constexpr char* ASSETS_LIVES_PATH = "../data/Images/UI/heart.png";
constexpr float POWER_UP_SECONDS_DURATION = 15.f;

// Enemy Constants
constexpr float ENEMY_RETURN_POSITION_TOLERANCE = 2.0f;
constexpr float FLYING_ENEMY_OSCILLATION_RANGE = 0.7f;
constexpr float FLYING_ENEMY_OSCILLATION_SPEED = 2.f;
constexpr float STATIC_ENEMY_WATCH_DURATION = 5000.0f;
constexpr float WALKING_ENEMY_IDLE_DURATION = 2000.0f;
constexpr float WALKING_ENEMY_PATROL_DURATION = 5000.0f;

constexpr float PRELOAD_ADVANCE_RANGE = 1000.f;

