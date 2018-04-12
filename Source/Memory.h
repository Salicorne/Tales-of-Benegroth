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

	public:
		MemoryManager();
		~MemoryManager();
		void addTexture(std::string path, id_t id);
		void addSprite(id_t texture, vec2f posInWorld, id_t id, float feetOffset = 0);
		void addTreeSprite(id_t trunk, id_t leaves, int offset, vec2f posInWorld, id_t id, float feetOffset = 0);
		void addAnimatedSprite(id_t texture, sf::IntRect rect, sf::Time duration, vec2f posInWorld, id_t id, float feetOffset = 0);
		void addAdditionalSprite(id_t texture, sf::IntRect rect, sf::Time duration, id_t id);
		SpriteSet* addSpriteSet(id_t texture, sf::IntRect rect, sf::Time duration, vec2f posInWorld, id_t id, float feetOffset = 0);
		void addRepeatedSprite(id_t texture, int w, int h, vec2f posInWorld, id_t id, float feetOffset = 0);
		Texture& getTexture(id_t id);
		Sprite& getSprite(id_t id);
		vec2f getWindowPos();
		void updateBlur(vec2f playerPos);

		void moveWindow(vec2f delta);
		void animateSprites(sf::Time elapsed);
		void sortSprites();
		void draw(sf::RenderTarget& window);
};

