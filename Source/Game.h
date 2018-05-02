#pragma once
#include "Memory.h"
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

class GameManager : public AbstractInteractionProvider {
	protected:
		MemoryManager& mgr;
		std::vector<Actor*> actors;
		Actor* player;
		sf::Window& window;
		std::string actionMessage;
		Actor* target;
		std::mutex actionMessageMutex;
		
		// AbstractInteractionProvider
		std::function<void(std::string, std::string)> showMessageFunction; 

	public:
		GameManager(MemoryManager& mgr, sf::Window& window);
		~GameManager();
		Actor* getPlayer();
		Actor* addActor(game_id sprite, vec2f pos, float speed, game_id id);
		NPC* addNPC(game_id sprite, vec2f pos, float speed, game_id id);
		void setPlayer(Actor* a);

		void movePlayer(vec2f delta, sf::Time elapsed);
		void updateActionMessage();
		std::string getActionMessage();
		void playActors(sf::Time elapsed);
		void interact();

		//AbstractInteractionProvider
		void setShowMessageFunction(std::function<void(std::string, std::string)> fun);
		virtual void showMessage(std::string sender, std::string message);
};

class AssetCreator {
	private:
		AssetCreator(){}
	
	public:
		static void createTreeSprite(MemoryManager& mgr, game_id trunk, game_id leaves, int minOffset, int maxOffset, vec2f posInWorld, game_id id, float feetOffset = 0);
		static void createSpriteSet(MemoryManager& mgr, game_id baseTexture, std::vector<std::pair<game_id, sf::Color>>, sf::IntRect rect, vec2f posInWorld, sf::Time animationTime, game_id id, float feetOffset = 0);
};
