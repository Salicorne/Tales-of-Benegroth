#include "Game.h"


GameManager::GameManager(MemoryManager& mgr) : mgr(mgr) {
	player = nullptr;
}

GameManager::~GameManager() {
	for (const Actor* a : actors) {
		delete a;
	}
}

Actor * GameManager::getPlayer() {
	return player;
}

Actor* GameManager::addActor(id_t sprite, vec2f pos, float speed, id_t id) {
	Actor* a = new Actor(static_cast<SpriteSet*>(&mgr.getSprite(sprite)), pos, speed, id);
	actors.push_back(a);
	return a;
}

NPC * GameManager::addNPC(id_t sprite, vec2f pos, float speed, id_t id) {
	NPC* a = new NPC(static_cast<SpriteSet*>(&mgr.getSprite(sprite)), pos, speed, id);
	actors.push_back(a);
	return a;
}

void GameManager::setPlayer(Actor * a) {
	player = a;
}

void GameManager::movePlayer(vec2f delta, sf::Time elapsed) {
	if (player != nullptr) {
		player->move(delta, elapsed);
		vec2f feet = player->getSpriteSet()->getFeetPos() - mgr.getWindowPos();
		vec2f head = player->getSpriteSet()->getHeadPos() - mgr.getWindowPos();
		vec2f windowMovement(0, 0);
		if (head.x < Game::Window::Width * Game::Window::widthScrollLimit) { windowMovement.x += head.x - Game::Window::Width * Game::Window::widthScrollLimit; }
		if (head.x > Game::Window::Width * (1 - Game::Window::widthScrollLimit)) { windowMovement.x += head.x - Game::Window::Width * (1 - Game::Window::widthScrollLimit); }
		if (head.y < Game::Window::Height * Game::Window::heightScrollLimit) { windowMovement.y += head.y - Game::Window::Height * Game::Window::heightScrollLimit; }
		if (feet.y > Game::Window::Height * (1 - Game::Window::heightScrollLimit)) { windowMovement.y += feet.y - Game::Window::Height * (1 - Game::Window::heightScrollLimit); }
		mgr.moveWindow(windowMovement);
	}
}

void GameManager::playActors(sf::Time elapsed) {
	for (Actor* a : actors) {
		a->action(elapsed);
	}
}
