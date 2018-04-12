#include "Actor.h"

Actor::Actor(SpriteSet* sprite, vec2f pos, float speed, game_id id) : sprite(sprite), posInWorld(pos), speed(speed), id(id) {
	sprite->setPosInWorld(pos);
	direction = Game::Direction::South;
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

SpriteSet* Actor::getSpriteSet() {
	return sprite;
}

NPC::NPC(SpriteSet* sprite, vec2f pos, float speed, game_id id) : Actor(sprite, pos, speed, id) {}

void NPC::action(sf::Time elapsed) {
	moveTo(vec2f(0, 0), elapsed);
}
