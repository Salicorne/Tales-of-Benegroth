#pragma once
#include "Graphics.h"

class AbstractInteractionProvider {
	public:
		virtual void showMessage(std::string sender, std::string message) = 0;
};

class Actor {
	protected:
		SpriteSet* sprite;
		vec2f posInWorld;
		game_id id;
		float speed;
		Game::Direction direction;
		bool possibleInteraction;

	public:
		Actor(SpriteSet* sprite, vec2f pos, float speed, game_id id);
		SpriteSet* getSpriteSet();
		bool canHaveInteraction();
		virtual std::string getInteractionMessage();
		virtual void move(vec2f delta, sf::Time elapsed);
		virtual void moveTo(vec2f pos, sf::Time elapsed);
		virtual void action(sf::Time elapsed);
		virtual void interact(AbstractInteractionProvider* aip);
};

class NPC : public Actor {
	protected:
		std::vector<std::pair<vec2f, sf::Time>> locations;
		int currentLocation;
		sf::Time locationCounter;
		
	public:
		NPC(SpriteSet* sprite, vec2f pos, float speed, game_id id);
		virtual void action(sf::Time elapsed);
		virtual void addLocation(vec2f p, sf::Time t);
		virtual std::string getInteractionMessage();
		virtual void interact(AbstractInteractionProvider* aip);
};

