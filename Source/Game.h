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
		Actor* addActor(game_id sprite, vec2f pos, float speed, game_id id);
		NPC* addNPC(game_id sprite, vec2f pos, float speed, game_id id);
		void setPlayer(Actor* a);

		void movePlayer(vec2f delta, sf::Time elapsed);
		void drawMessage(std::string message);

		void playActors(sf::Time elapsed);
};

class AssetCreator {
	private:
		AssetCreator(){}
	
	public:
		static void createTreeSprite(MemoryManager& mgr, game_id trunk, game_id leaves, int minOffset, int maxOffset, vec2f posInWorld, game_id id, float feetOffset = 0);
		static void createSpriteSet(MemoryManager& mgr, game_id baseSprite, std::vector<std::pair<game_id, sf::Color>>, sf::IntRect rect, vec2f posInWorld, sf::Time animationTime, game_id id, float feetOffset = 0);
};
