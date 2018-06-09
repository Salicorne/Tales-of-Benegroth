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

bool Actor::isAlive() {
	return life > 0;
}

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
		}
		moveTo(locations.at(currentLocation).first, elapsed, acm);
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

Actor* AttackableActor::getTarget() {
	int maxThreat = 0;
	Actor* target = nullptr;
	int i = 0, j = 0;
	threatTableMutex.lock();
	for (auto& a : threatTable) {
		if (a.second > maxThreat) {
			target = a.first;
			maxThreat = a.second;
			j = i;
		}
		i++;
	}
	threatTableMutex.unlock();
	if (target != nullptr && !target->isAlive()) {
		threatTableMutex.lock();
		threatTable.erase(threatTable.begin() + j);
		threatTableMutex.unlock();
		target = getTarget();
	}
	return target;
}

Game::MobAction Mob::chooseAttack() {
	return Game::MobAction::Attack1;
}

Mob::Mob(SpriteSet* sprite, vec2f pos, float speed, int life, game_id id) : AttackableActor(sprite, pos, speed, life, id), currentAction(Game::MobAction::Idle), initialPos(pos), randomPos(pos) {}

void Mob::onIdle(sf::Time elapsed, AbstractCollisionsManager& acm) {
	vec2f pos = this->getSpriteSet()->getPosInWorld();
	if (norm(randomPos - pos) < 10.f) {
		clock += elapsed;
	}
	moveTo(randomPos, elapsed, acm);
	if (clock >= sf::seconds(5)) {
		int range = 200;
		randomPos = vec2f(initialPos.x - range / 2.f + rand() % range, initialPos.y - range / 2.f + rand() % range);
		std::cout << "Moving to " << randomPos.x << ";" << randomPos.y << std::endl;
		clock = sf::Time::Zero;
	}

	// React if the mob is attacked
	Actor* target = getTarget();
	if (target != nullptr) {
		currentAction = chooseAttack();
		clock = sf::Time::Zero;
	}
}

void Mob::onAttack1(sf::Time elapsed, AbstractCollisionsManager& acm) {
	Actor* target = getTarget();
	vec2f pos = this->getSpriteSet()->getPosInWorld();
	if (target != nullptr) {
		vec2f targetPos = target->getSpriteSet()->getPosInWorld();
		if (norm(pos - targetPos) > 500.f) {
			// Reset
			forgetTarget();
		}
		// Move
		if (norm(targetPos - pos) > 100.f) {
			moveTo(targetPos, elapsed, acm);
		}
		// Attack
		else {
			moveTo(pos, elapsed, acm);
		}
	}
	else { currentAction = Game::MobAction::Idle; }
}

void Mob::forgetTarget() {
	Actor* target = getTarget();
	threatTableMutex.lock();
	if (target != nullptr) {
		int i = 0;
		for (auto& a : threatTable) {
			if (a.first == target) {
				threatTable.erase(threatTable.begin() + i);
				threatTableMutex.unlock();
				return;
			}
			i++;
		}
	}
	threatTableMutex.unlock();
}

void Mob::action(sf::Time elapsed, AbstractCollisionsManager& acm) {
	switch (currentAction) {
		case Game::MobAction::Idle:
		{
			onIdle(elapsed, acm);
			break;
		}
		case Game::MobAction::Attack1:
		{
			onAttack1(elapsed, acm);
			break;
		}
	}
	//print();
}
