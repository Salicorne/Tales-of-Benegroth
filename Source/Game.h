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
		void drawMessage(std::string message);

		void playActors(sf::Time elapsed);
};

class AssetCreator {
	private:
		AssetCreator(){}
	
	public:
		static void createTreeSprite(MemoryManager& mgr, id_t trunk, id_t leaves, int minOffset, int maxOffset, vec2f posInWorld, id_t id, float feetOffset = 0);
		static void createSpriteSet(MemoryManager& mgr, id_t baseSprite, std::vector<std::pair<id_t, sf::Color>>, sf::IntRect rect, vec2f posInWorld, sf::Time animationTime, id_t id, float feetOffset = 0);
};
