#include "Actor.h"

Actor::Actor(SpriteSet* sprite, vec2f pos, float speed, int life, game_id id) : sprite(sprite), posInWorld(pos), speed(speed), maxLife(life), life(life), id(id), possibleInteraction(false) {
	sprite->setPosInWorld(pos);
	direction = Game::Direction::South;
}

bool Actor::canHaveInteraction() {
	return possibleInteraction;
}

int Actor::getLife() { return life; }

int Actor::getMaxLife() { return maxLife; }

int Actor::getDamage(int baseDamage, Actor* sender) {
	//todo: add modifiers
	this->life = std::max(this->life - baseDamage, 0);
	return baseDamage;
}

void Actor::move(vec2f delta, sf::Time elapsed, AbstractCollisionsManager& acm) {
	delta = normalize(delta);
	delta *= speed;
	delta *= elapsed.asMilliseconds() / 10.f;
	// dead axis
	if (abs(delta.x) < 0.1) { delta.x = 0; }
	if (abs(delta.y) < 0.1) { delta.y = 0; }
	if(delta.x != 0 || delta.y != 0) {
		if(acm.collides(posInWorld + delta)) {  // In case of collision
			// Slide
			if(acm.collides(vec2f(posInWorld.x + delta.x, posInWorld.y))) { delta.x = 0; }
			if(acm.collides(vec2f(posInWorld.x, posInWorld.y + delta.y))) { delta.y = 0; }
		}
	}
	this->sprite->move(delta);
	this->posInWorld += delta;
	direction = getDirectionFromVectorWithDefault(delta, direction);
	this->sprite->setAnimation(norm(delta) > 0 ? getWalkAnimationFromDirection(direction) : getIdleAnimationFromDirection(direction));
}

void Actor::moveTo(vec2f pos, sf::Time elapsed, AbstractCollisionsManager& acm) {
	move(norm(pos - posInWorld) > 1 ? pos - posInWorld : vec2f(0,0), elapsed, acm);
}

void Actor::action(sf::Time elapsed, AbstractCollisionsManager& acm) {}

void Actor::interact(AbstractInteractionProvider* aip) {}

std::string Actor::getInteractionMessage() {
	return "...";
}

SpriteSet* Actor::getSpriteSet() {
	return sprite;
}

NPC::NPC(SpriteSet* sprite, vec2f pos, float speed, int life, game_id id) : Actor(sprite, pos, speed, life, id), currentLocation(0) {
	possibleInteraction = true;
}

void NPC::action(sf::Time elapsed, AbstractCollisionsManager& acm) {
	if(locations.size() > 0 && life > 0) {
		if(norm(vec2f(posInWorld.x - locations.at(currentLocation).first.x, posInWorld.y - locations.at(currentLocation).first.y)) < 1) {
			// NPC is standing in a location spot
			locationCounter -= elapsed;
			if(locationCounter <= sf::Time::Zero) {
				// Change current location
				currentLocation = (currentLocation + 1) % locations.size();
				locationCounter = locations.at(currentLocation).second;
			}
			move(vec2f(0,0), elapsed, acm);
		}
		else {
			// Move
			moveTo(locations.at(currentLocation).first, elapsed, acm);
		}
	}
}

void NPC::interact(AbstractInteractionProvider* aip) {
	aip->showMessage("Sample NPC", "Hi ! I am your first NPC. I don't have so much to tell you right now, I am only supposed to check these three locations. But don't hesitate to come back later, when interactions will be a little more developped ! ");
}

std::string NPC::getInteractionMessage() {
	return "E : Parler";
}

void NPC::addLocation(vec2f p, sf::Time t) {
	if(locations.size() == 0) {
		locationCounter = t;
	}
	locations.push_back(std::pair<vec2f, sf::Time>(p, t));
}

void AttackableActor::print() {
	std::cout << "-----------------" << std::endl;
	for (auto& a : threatTable) {
		std::cout << a.first << " : " << a.second << std::endl;
	}
	std::cout << "life : " << life << std::endl;
	std::cout << "-----------------" << std::endl;
}

AttackableActor::AttackableActor(SpriteSet* sprite, vec2f pos, float speed, int life, game_id id) : Actor(sprite, pos, speed, life, id) {}

int AttackableActor::getDamage(int baseDamage, Actor* sender) {
	int realDamage = Actor::getDamage(baseDamage, sender);
	// Update existing target
	threatTableMutex.lock();
	for (auto& a : threatTable) {
		if (a.first == sender) {
			a.second += realDamage;
			threatTableMutex.unlock();
			print();
			return realDamage;
		}
	}
	// Insert target
	threatTable.push_back(std::make_pair(sender, realDamage));
	threatTableMutex.unlock();
	print();
	return realDamage;
}

Mob::Mob(SpriteSet* sprite, vec2f pos, float speed, int life, game_id id) : AttackableActor(sprite, pos, speed, life, id) {}
