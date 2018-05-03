#pragma once
#include "Actor.h"

class MemoryManager {
	protected:
		std::vector<Texture*> textures;
		std::vector<Sprite*> sprites;
		Texture defaultTexture;
		Sprite defaultSprite;
		vec2f windowPosition;
		std::mutex spritesMutex;
		std::mutex texturesMutex;
		game_id counter;

	public:
		MemoryManager();
		~MemoryManager();
		game_id getFreeSpriteId();
		void addTexture(std::string path, game_id id);
		void addSprite(game_id texture, vec2f posInWorld, game_id id, float feetOffset = 0);
		void addTreeSprite(game_id trunk, game_id leaves, int offset, vec2f posInWorld, game_id id, float feetOffset = 0);
		void addAnimatedSprite(game_id texture, sf::IntRect rect, sf::Time duration, vec2f posInWorld, game_id id, float feetOffset = 0);
		void addAdditionalSprite(game_id texture, sf::IntRect rect, sf::Time duration, game_id id);
		SpriteSet* addSpriteSet(game_id texture, sf::IntRect rect, sf::Time duration, vec2f posInWorld, game_id id, float feetOffset = 0);
		void addRepeatedSprite(game_id texture, int w, int h, vec2f posInWorld, game_id id, float feetOffset = 0);
		bool containsTexture(game_id id);
		Texture& getTexture(game_id id);
		Sprite& getSprite(game_id id);
		vec2f getWindowPos();
		void updateBlur(vec2f playerPos);
		void dropSprites();

		void moveWindow(vec2f delta);
		void animateSprites(sf::Time elapsed);
		void sortSprites();
		void draw(sf::RenderTarget& window, sf::Shader* shader = nullptr);
};

