#pragma once
#include "Graphics.h"

class AbstractInteractionProvider {
	public:
		virtual void showMessage(std::string sender, std::string message) = 0;
};

class AbstractCollisionsManager {
	public:
		virtual bool collides(vec2f point) = 0;
};

class Actor {
	protected:
		SpriteSet* sprite;
		vec2f posInWorld;
		game_id id;
		float speed;
		Game::Direction direction;
		bool possibleInteraction;
		int maxLife;
		int life;

	public:
		Actor(SpriteSet* sprite, vec2f pos, float speed, int life, game_id id);
		SpriteSet* getSpriteSet();
		bool canHaveInteraction();
		int getLife();
		int getMaxLife();
		bool isAlive();
		virtual int getDamage(int baseDamage, Actor* sender);
		virtual std::string getInteractionMessage();
		virtual void move(vec2f delta, sf::Time elapsed, AbstractCollisionsManager& acm);
		virtual void moveTo(vec2f pos, sf::Time elapsed, AbstractCollisionsManager& acm);
		virtual void action(sf::Time elapsed, AbstractCollisionsManager& acm);
		virtual void interact(AbstractInteractionProvider* aip);
};

class NPC : public Actor {
	protected:
		std::vector<std::pair<vec2f, sf::Time>> locations;
		int currentLocation;
		sf::Time locationCounter;
		
	public:
		NPC(SpriteSet* sprite, vec2f pos, float speed, int life, game_id id);
		virtual void action(sf::Time elapsed, AbstractCollisionsManager& acm);
		virtual void addLocation(vec2f p, sf::Time t);
		virtual std::string getInteractionMessage();
		virtual void interact(AbstractInteractionProvider* aip);
};

class AttackableActor : public Actor {
	protected:
		std::vector<std::pair<Actor*, int>> threatTable;
		sf::Mutex threatTableMutex;
		void print();

	public:
		AttackableActor(SpriteSet* sprite, vec2f pos, float speed, int life, game_id id);
		virtual int getDamage(int baseDamage, Actor* sender);
		Actor* getTarget();
};

class Mob : public AttackableActor {
	protected:
		Game::MobAction currentAction;
		sf::Time clock;
		vec2f initialPos;
		vec2f randomPos;
		Game::MobAction chooseAttack();
		virtual void onIdle(sf::Time elapsed, AbstractCollisionsManager& acm);
		virtual void onAttack1(sf::Time elapsed, AbstractCollisionsManager& acm);
		void forgetTarget();

	public:
		Mob(SpriteSet* sprite, vec2f pos, float speed, int life, game_id id);
		virtual void action(sf::Time elapsed, AbstractCollisionsManager& acm);
};

