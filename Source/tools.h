#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
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
	extern int joystick;
	extern int currentScreen;
	const float minDistanceForAction = 100.f;

	enum Screen {
		Menu = 0, 
		Game = 1
	};

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

	enum FileOp {
		Texture = 0, 
		Sprite = 1, 
		RepeatedSprite = 2, 
		AnimatedSprite = 3, 
		TreeSprite = 4, 
		SpriteSet = 5, 
		Collision = 15, 
		SetLayer = 21, 
		SetBlur = 22, 
		SetPlayer = 23, 
		AddNPC = 24
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