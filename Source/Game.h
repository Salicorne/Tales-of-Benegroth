#pragma once
#include "Memory.h"

class GameManager {
	protected:
		MemoryManager& mgr;
		std::vector<Actor*> actors;
		Actor* player;

	public:
		GameManager(MemoryManager& mgr);
		~GameManager();
		Actor* getPlayer();
		Actor* addActor(id_t sprite, vec2f pos, float speed, id_t id);
		NPC* addNPC(id_t sprite, vec2f pos, float speed, id_t id);
		void setPlayer(Actor* a);
		void movePlayer(vec2f delta, sf::Time elapsed);

		void playActors(sf::Time elapsed);
};

class AssetCreator {
	private:
		AssetCreator(){}
	
	public:
		//static void createTreeSprite();
};
