#include "Actor.h"

Actor::Actor(SpriteSet* sprite, vec2f pos, float speed, game_id id) : sprite(sprite), posInWorld(pos), speed(speed), id(id), possibleInteraction(false) {
	sprite->setPosInWorld(pos);
	direction = Game::Direction::South;
}

bool Actor::canHaveInteraction() {
	return possibleInteraction;
}

void Actor::move(vec2f delta, sf::Time elapsed) {
	delta = normalize(delta);
	delta *= speed;
	delta *= elapsed.asMilliseconds() / 10.f;
	// dead axis
	if (abs(delta.x) < 0.1) { delta.x = 0; }
	if (abs(delta.y) < 0.1) { delta.y = 0; }
	this->sprite->move(delta);
	this->posInWorld += delta;
	direction = getDirectionFromVectorWithDefault(delta, direction);
	this->sprite->setAnimation(norm(delta) > 0 ? getWalkAnimationFromDirection(direction) : getIdleAnimationFromDirection(direction));
}

void Actor::moveTo(vec2f pos, sf::Time elapsed) {
	move(norm(pos - posInWorld) > 1 ? pos - posInWorld : vec2f(0,0), elapsed);
}

void Actor::action(sf::Time elapsed) {}

void Actor::interact(AbstractInteractionProvider* aip) {}

std::string Actor::getInteractionMessage() {
	return "...";
}

SpriteSet* Actor::getSpriteSet() {
	return sprite;
}

NPC::NPC(SpriteSet* sprite, vec2f pos, float speed, game_id id) : Actor(sprite, pos, speed, id), currentLocation(0) {
	possibleInteraction = true;
}

void NPC::action(sf::Time elapsed) {
	if(locations.size() > 0) {
		if(norm(vec2f(posInWorld.x - locations.at(currentLocation).first.x, posInWorld.y - locations.at(currentLocation).first.y)) < 1) {
			// NPC is standing in a location spot
			locationCounter -= elapsed;
			if(locationCounter <= sf::Time::Zero) {
				// Change current location
				currentLocation = (currentLocation + 1) % locations.size();
				locationCounter = locations.at(currentLocation).second;
			}
			move(vec2f(0,0), elapsed);
		}
		else {
			// Move
			moveTo(locations.at(currentLocation).first, elapsed);
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
