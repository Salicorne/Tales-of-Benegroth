#include "Game.h"


GameManager::GameManager(MemoryManager& mgr, sf::Window& window) : mgr(mgr), window(window) {
	player = nullptr;
	target = nullptr;
	showMessageFunction = [](std::string, std::string){};
}

GameManager::~GameManager() {
	for (const Actor* a : actors) {
		delete a;
	}
}

Actor * GameManager::getPlayer() {
	return player;
}

Actor* GameManager::addActor(game_id sprite, vec2f pos, float speed, game_id id) {
	Actor* a = new Actor(static_cast<SpriteSet*>(&mgr.getSprite(sprite)), pos, speed, id);
	actors.push_back(a);
	return a;
}

NPC * GameManager::addNPC(game_id sprite, vec2f pos, float speed, game_id id) {
	NPC* a = new NPC(static_cast<SpriteSet*>(&mgr.getSprite(sprite)), pos, speed, id);
	actors.push_back(a);
	return a;
}

void GameManager::setPlayer(Actor * a) {
	player = a;
}

void GameManager::addCollision(sf::FloatRect c) {
	collisions.push_back(c);
}

void GameManager::movePlayer(vec2f delta, sf::Time elapsed) {
	if (player != nullptr) {
		player->move(delta, elapsed, *this);
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

void GameManager::updateActionMessage() {
	if(player == nullptr) { return; }
	for(Actor* a : actors) {
		if(a != player && a->canHaveInteraction() && norm(a->getSpriteSet()->getPosInWorld() - player->getSpriteSet()->getPosInWorld()) < Game::minDistanceForAction) {
			target = a;
			actionMessageMutex.lock();
			actionMessage = a->getInteractionMessage();
			actionMessageMutex.unlock();
			return;
		}
	}
	target = nullptr;
	actionMessage = "";
}

std::string GameManager::getActionMessage() {
	actionMessageMutex.lock();
	std::string m = actionMessage;
	actionMessageMutex.unlock();
	return m;
}

void GameManager::playActors(sf::Time elapsed) {
	for (Actor* a : actors) {
		a->action(elapsed);
	}
}

void GameManager::interact() {
	if(target != nullptr) {
		//todo
		target->interact(this);
	}
}

void GameManager::setShowMessageFunction(std::function<void(std::string, std::string)> fun) {
	showMessageFunction = fun;
}

void GameManager::showMessage(std::string sender, std::string message) {
	showMessageFunction(sender, message);
}

bool GameManager::collides(vec2f point) {
	for(sf::FloatRect& c : collisions) {
		if(c.contains(point)) {return true;}
	}
	return false;
}




void AssetCreator::createTreeSprite(MemoryManager& mgr, game_id trunk, game_id leaves, int minOffset, int maxOffset, vec2f posInWorld, game_id id, float feetOffset) {
	mgr.addTreeSprite(trunk, leaves, minOffset + (rand() % (maxOffset - minOffset + 1)), posInWorld, id, feetOffset);
}

void AssetCreator::createSpriteSet(MemoryManager& mgr, game_id baseTexture, std::vector<std::pair<game_id, sf::Color>> additions, sf::IntRect rect, vec2f posInWorld, sf::Time animationTime, game_id id, float feetOffset) {
	for (auto& i : additions) {
		game_id uid = mgr.getFreeSpriteId();
		mgr.addAdditionalSprite(i.first, rect, animationTime, uid);
		mgr.getSprite(uid).setColor(i.second);
		i.first = uid;
	}
	SpriteSet* s = mgr.addSpriteSet(baseTexture, rect, animationTime, posInWorld, id, feetOffset);
	for (const auto i : additions) {
		s->addAdditionalSprite(static_cast<AdditionalSprite&>(mgr.getSprite(i.first)));
	}
}
