#pragma once
#include <list>
#include "Object.hpp"

// Game
#define GAME_NAME "Square Dance"

#define UPDATES_PER_SECOND 60.f
#define FRAME_TIME 1.f / UPDATES_PER_SECOND

#define VIEW_CENTER sf::Vector2f(400, 140)
#define WORLD_SIZE sf::Vector2f(800, 280)

#define PLAYER_RADIUS 14.1421356f

#define INITIAL_VIEW_ZOOM 0.02f
#define MENU_VIEW_ZOOM 0.5f
#define GAME_VIEW_ZOOM 1.f

#define INITIAL_PLAYER_POTSITION sf::Vector2f(400.f, 140.f)
#define MENU_PLAYER_POSITION sf::Vector2f(400.f, 140.f)
#define GAME_PLAYER_POSITION sf::Vector2f(400.f, 140.f)

#define MENU_PLAYER_SCALE sf::Vector2f(1.f, 1.f)

#define INITIAL_WORLD_COLOR sf::Color(255, 255, 255, 0)
#define MENU_WORLD_COLOR sf::Color(255, 255, 255, 0)
#define GAME_WORLD_COLOR sf::Color(255, 255, 255, 10)

#define INITIAL_TITLE_COLOR sf::Color(242, 227, 160, 0)
#define MENU_TITLE_COLOR sf::Color(242, 227, 160, 255)
#define GAME_TITLE_COLOR sf::Color(242, 227, 160, 0)

#define MIN_INTRO_DURATION 3.f
#define TO_MENU_TRANSITION_DURATION 2.f
#define TO_GAME_TRANSITION_DURATION 2.f
#define PREPARING_DURATION 0.85f
#define PLAYER_REVIVAL_DURATION 2.2f

#define BACKGROUND_COLOR sf::Color(25, 25, 25, 255)
#define BACKGROUND_PARTITION_NUM 6.f

// Math
#define PI 3.141592653f


#define CHARACTER_SIZE_FACTOR (1.f / 480.f)
#define TITLE_CHARACTER_SIZE 61.f
#define TITLE_Y_POSITION_FACTOR 0.33125f
#define RADIAL_SHADER_FACTOR 0.0625f

// Particles
#define POINT_SIZE_FACTOR 0.00125f
#define VERTICES_DIVERSITY_COUNT 3

// Spawner
#define MAX_SPAWNER_QUEUE 5
#define FIRST_SPAWN_TIME_DELAY 2.f


// Containers
using GameObjects = std::list<Object*>;