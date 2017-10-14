#pragma once

// Game
#define GAME_NAME "Square Dance"

#define UPDATES_PER_SECOND 60

#define VIEW_CENTER sf::Vector2f(400, 140)
#define WORLD_SIZE sf::Vector2f(800, 280)

#define INITIAL_VIEW_ZOOM 8.f
#define MENU_VIEW_ZOOM 0.5f
#define GAME_VIEW_ZOOM 1.f

#define INITIAL_PLAYER_POTSITION sf::Vector2f(400.f, 140.f)
#define MENU_PLAYER_POSITION sf::Vector2f(400.f, 140.f)
#define GAME_PLAYER_POSITION sf::Vector2f(400.f, 140.f)

#define INITIAL_WORLD_COLOR sf::Color(255, 255, 255, 0)
#define MENU_WORLD_COLOR sf::Color(255, 255, 255, 0)
#define GAME_WORLD_COLOR sf::Color(255, 255, 255, 10)

#define INITIAL_TITLE_COLOR sf::Color(242, 227, 160, 0)
#define MENU_TITLE_COLOR sf::Color(242, 227, 160, 255)
#define GAME_TITLE_COLOR sf::Color(242, 227, 160, 0)

// Math
#define PI 3.14159265358979f


#define CHARACTER_SIZE_FACTOR (61.f / 480.f)
#define TITLE_Y_POSITION_FACTOR 0.33125f
#define RADIAL_SHADER_FACTOR 0.0625f

// Particles
#define POINT_SIZE_FACTOR 0.00125f

// Spawner
#define MAX_SPAWNER_QUEUE_SIZE 5
#define FIRST_SPAWN_TIME_DELAY 2.f