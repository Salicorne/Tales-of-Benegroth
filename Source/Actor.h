#pragma once
#include "Graphics.h"

class Actor {
	protected:
		SpriteSet* sprite;
		vec2f posInWorld;
		id_t id;
		float speed;
		Game::Direction direction;

	public:
		Actor(SpriteSet* sprite, vec2f pos, float speed, id_t id);
		SpriteSet* getSpriteSet();
		virtual void move(vec2f delta, sf::Time elapsed);
		virtual void moveTo(vec2f pos, sf::Time elapsed);
		virtual void action(sf::Time elapsed);
};

class NPC : public Actor {
	public:
		NPC(SpriteSet* sprite, vec2f pos, float speed, id_t id);
		virtual void action(sf::Time elapsed);
};
