#include "tools.h"

float norm(vec2f vec) { return sqrt(vec.x*vec.x + vec.y*vec.y); }

vec2f normalize(vec2f vec) {
	float n = norm(vec);
	if (n < 0.01f) { return vec2f(0, 0); }
	return vec2f(vec.x / n, vec.y / n);
}

int Game::Window::Width;
int Game::Window::Height;
float Game::Window::widthScrollLimit;
float Game::Window::heightScrollLimit;
int Game::joystick;
int Game::currentScreen;

Game::Animation getIdleAnimationFromDirection(Game::Direction dir) {
	switch (dir) {
		case Game::Direction::North:
			return Game::Animation::IdleN;
		case Game::Direction::South:
			return Game::Animation::IdleS;
		case Game::Direction::East:
			return Game::Animation::IdleE;
		case Game::Direction::West:
			return Game::Animation::IdleW;
		default:
			return Game::Animation::IdleN;
	}
}

Game::Animation getWalkAnimationFromDirection(Game::Direction dir) {
	switch (dir) {
	case Game::Direction::North:
		return Game::Animation::WalkN;
	case Game::Direction::South:
		return Game::Animation::WalkS;
	case Game::Direction::East:
		return Game::Animation::WalkE;
	case Game::Direction::West:
		return Game::Animation::WalkW;
	default:
		return Game::Animation::WalkN;
	}
}

Game::Direction getDirectionFromVectorWithDefault(vec2f vec, Game::Direction def) {
	float n = norm(vec);
	if (n != 0) {
		if (vec.x / n > sqrt(2) / 2) { return Game::Direction::East; }
		if (vec.x / n < -1 * sqrt(2) / 2) { return Game::Direction::West; }
	}
	if (vec.x == 0 && vec.y == 0) { return def; }
	return (vec.y > 0) ? Game::Direction::South : Game::Direction::North;
}
