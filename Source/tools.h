#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <thread>
#include <mutex>

typedef int game_id;
typedef sf::Vector2f vec2f;

float norm(vec2f vec);
vec2f normalize(vec2f vec);

namespace Game {
	enum Animation {
		IdleN = 0, 
		IdleW = 1, 
		IdleS = 2, 
		IdleE = 3,
		WalkN = 4, 
		WalkW = 5, 
		WalkS = 6, 
		WalkE = 7
	};

	enum Direction {
		North, 
		South, 
		East, 
		West
	};

	namespace Window {
		extern int Width;
		extern int Height;
		extern float widthScrollLimit;
		extern float heightScrollLimit;
	};
}

Game::Animation getIdleAnimationFromDirection(Game::Direction dir);
Game::Animation getWalkAnimationFromDirection(Game::Direction dir);
Game::Direction getDirectionFromVectorWithDefault(vec2f vec, Game::Direction def);