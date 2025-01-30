cmake_minimum_required(VERSION 3.23)
project(Ghosty LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# Download and add SFML as a dependency
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Download and add tmxlite (tiled editor loader) as a dependency
FetchContent_Declare(
    TMXLITE
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG v1.4.4
    SOURCE_SUBDIR tmxlite)
FetchContent_MakeAvailable(TMXLITE)

# Download and add nlohmann (json library) as a dependency
FetchContent_Declare(
    JSON
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2 # Reemplaza con la versión que prefieras
)
FetchContent_MakeAvailable(JSON)


set(PROJECT_HEADERS 

    include/Core/AssetManager.h
    include/Core/Game.h
    include/Core/World.h
    include/Core/Level.h

    include/Gameplay/AttackSystem/Attack.h
    include/Gameplay/AttackSystem/MeleeAttack.h
    include/Gameplay/AttackSystem/Projectile.h
    include/Gameplay/AttackSystem/RangedAttack.h
    include/Gameplay/Collectibles/Collectible.h
    include/Gameplay/Collectibles/CollectibleFactory.h
    include/Gameplay/Collectibles/CollectibleManager.h
    include/Gameplay/Collectibles/CollectibleType.h
    include/Gameplay/Collisions/Collider.h
    include/Gameplay/Collisions/Collisionable.h
    include/Gameplay/Collisions/CollisionManager.h
    include/Gameplay/Enemy/DinoEnemy.h
    include/Gameplay/Enemy/Enemy.h
    include/Gameplay/Enemy/EnemyFactory.h
    include/Gameplay/Enemy/EnemyManager.h
    include/Gameplay/Enemy/EnemyType.h
    include/Gameplay/Enemy/FlyingEnemy.h
    include/Gameplay/Enemy/GhostEnemy.h
    include/Gameplay/Enemy/SkeletonEnemy.h
    include/Gameplay/Enemy/StaticEnemy.h
    include/Gameplay/Enemy/VampireEnemy.h
    include/Gameplay/Enemy/WalkingEnemy.h
    include/Gameplay/Player/Player.h
    include/Gameplay/Player/PlayerFactory.h

    include/Gameplay/Entity.h
    include/Gameplay/EntityLife.h
    include/Gameplay/GameObject.h

    include/Render/Animation.h
    include/Render/AnimationLoader.h
    include/Render/AnimationType.h
    include/Render/SFMLOrthogonalLayer.h

    include/UI/HUD.h
    include/UI/UIManager.h
    include/UI/UIScreen.h
    include/UI/UIScreenStaticBackground.h
    include/UI/UIScreenMainMenu.h
    include/UI/UIScreenPlaying.h

    include/Utils/Constants.h
    include/Utils/GameConfigLoader.h
    include/Utils/ObjectPool.h
)

set(PROJECT_SOURCES 

    src/Core/AssetManager.cpp
    src/Core/Game.cpp
    src/Core/World.cpp
    src/Core/Level.cpp

    src/Gameplay/AttackSystem/MeleeAttack.cpp
    src/Gameplay/AttackSystem/Projectile.cpp
    src/Gameplay/AttackSystem/RangedAttack.cpp
    src/Gameplay/Collectibles/Collectible.cpp
    src/Gameplay/Collectibles/CollectibleFactory.cpp
    src/Gameplay/Collectibles/CollectibleManager.cpp
    src/Gameplay/Collisions/Collider.cpp
    src/Gameplay/Collisions/Collisionable.cpp
    src/Gameplay/Collisions/CollisionManager.cpp
    src/Gameplay/Enemy/DinoEnemy.cpp
    src/Gameplay/Enemy/Enemy.cpp
    src/Gameplay/Enemy/EnemyFactory.cpp
    src/Gameplay/Enemy/EnemyManager.cpp
    src/Gameplay/Enemy/FlyingEnemy.cpp
    src/Gameplay/Enemy/GhostEnemy.cpp
    src/Gameplay/Enemy/SkeletonEnemy.cpp
    src/Gameplay/Enemy/StaticEnemy.cpp
    src/Gameplay/Enemy/VampireEnemy.cpp
    src/Gameplay/Enemy/WalkingEnemy.cpp

    src/Gameplay/Player/Player.cpp
    src/Gameplay/Player/PlayerFactory.cpp

    src/Gameplay/Entity.cpp
    src/Gameplay/EntityLife.cpp

    src/Render/Animation.cpp
    src/Render/AnimationLoader.cpp

    src/UI/HUD.cpp
    src/UI/UIManager.cpp
    src/UI/UIScreenStaticBackground.cpp
    src/UI/UIScreenMainMenu.cpp
    src/UI/UIScreenPlaying.cpp

    src/Utils/GameConfigLoader.cpp

    src/main.cpp
)

set(PROJECT_FILES 
  ${PROJECT_HEADERS} 
  ${PROJECT_SOURCES}
)

include_directories(${PROJECT_NAME} PRIVATE ${tmxlite_INCLUDE_DIR} include)

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics tmxlite nlohmann_json::nlohmann_json)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG_MODE=0)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()
