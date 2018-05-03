#include "Memory.h"



MemoryManager::MemoryManager() : defaultTexture("", 0), defaultSprite(defaultTexture, vec2f(0, 0), 0, 1), windowPosition(0, 0) {
}


MemoryManager::~MemoryManager() {
	spritesMutex.lock();
	for (const Sprite* spr : sprites) {
		delete spr;
	}
	spritesMutex.unlock();
	texturesMutex.lock();
	for (const Texture* txt : textures) {
		delete txt;
	}
	texturesMutex.unlock();
}

game_id MemoryManager::getFreeSpriteId() {
	counter++;
	return 10000 + counter;
}

void MemoryManager::addTexture(std::string path, game_id id) {
	texturesMutex.lock();
	this->textures.push_back(new Texture(path, id));
	texturesMutex.unlock();
}

void MemoryManager::addSprite(game_id texture, vec2f posInWorld, game_id id, float feetOffset) {
	spritesMutex.lock();
	sprites.push_back(new Sprite(getTexture(texture), posInWorld, id, feetOffset));
	spritesMutex.unlock();
}

void MemoryManager::addTreeSprite(game_id trunk, game_id leaves, int offset, vec2f posInWorld, game_id id, float feetOffset) {
	spritesMutex.lock();
	TreeSprite* tree = new TreeSprite(getTexture(trunk), getTexture(leaves), offset, posInWorld, id, feetOffset);
	tree->blurs(true);
	sprites.push_back(tree);
	spritesMutex.unlock();
}

void MemoryManager::addAnimatedSprite(game_id texture, sf::IntRect rect, sf::Time duration, vec2f posInWorld, game_id id, float feetOffset) {
	spritesMutex.lock();
	sprites.push_back(new AnimatedSprite(getTexture(texture), rect, duration, posInWorld, id, feetOffset));
	spritesMutex.unlock();
}

void MemoryManager::addAdditionalSprite(game_id texture, sf::IntRect rect, sf::Time duration, game_id id) {
	spritesMutex.lock();
	sprites.push_back(new AdditionalSprite(getTexture(texture), rect, duration, id));
	spritesMutex.unlock();
}

SpriteSet* MemoryManager::addSpriteSet(game_id texture, sf::IntRect rect, sf::Time duration, vec2f posInWorld, game_id id, float feetOffset) {
	SpriteSet* res = new SpriteSet(getTexture(texture), rect, duration, posInWorld, id, feetOffset);
	spritesMutex.lock();
	sprites.push_back(res);
	spritesMutex.unlock();
	return res;
}

void MemoryManager::addRepeatedSprite(game_id texture, int w, int h, vec2f posInWorld, game_id id, float feetOffset) {
	spritesMutex.lock();
	sprites.push_back(new RepeatedSprite(getTexture(texture), w, h, posInWorld, id, feetOffset));
	spritesMutex.unlock();
}

Texture& MemoryManager::getTexture(game_id id) {
	Texture* t = &defaultTexture;
	texturesMutex.lock();
	for (Texture* txt : textures) {
		if (txt->getId() == id) { t = txt; }
	}
	texturesMutex.unlock();
	return *t;
}

Sprite& MemoryManager::getSprite(game_id id) {
	Sprite* s = &defaultSprite;
	spritesMutex.lock();
	for (Sprite* spr : sprites) {
		if (spr->getId() == id) { s = spr; }
	}
	spritesMutex.unlock();
	return *s;
}

vec2f MemoryManager::getWindowPos() {
	return windowPosition;
}

void MemoryManager::updateBlur(vec2f playerPos) {
	spritesMutex.lock();
	for (Sprite* spr : sprites) {
		if (spr->blurs()) { spr->blur(sf::FloatRect(spr->getGlobalBounds().left, spr->getGlobalBounds().top, spr->getGlobalBounds().width, spr->getGlobalBounds().height - spr->getFeetOffset()).contains(playerPos - windowPosition)); }
	}
	spritesMutex.unlock();
}

void MemoryManager::dropSprites() {
	spritesMutex.lock();
	for (Sprite* spr : sprites) {
		delete spr;
	}
	sprites.clear();
	spritesMutex.unlock();
}

void MemoryManager::moveWindow(vec2f delta) {
	windowPosition += delta;
}

void MemoryManager::animateSprites(sf::Time elapsed) {
	spritesMutex.lock();
	for (Sprite* s : sprites) {
		s->animate(elapsed);
	}
	spritesMutex.unlock();
}

void MemoryManager::sortSprites() {
	spritesMutex.lock();
	std::sort(sprites.begin(), sprites.end(), [](const Sprite* a, const Sprite* b) -> bool { return (a->getLayer() == b->getLayer()) ? a->getFeetPos().y < b->getFeetPos().y : a->getLayer() < b->getLayer(); });
	spritesMutex.unlock();
}

void MemoryManager::draw(sf::RenderTarget& window) {
	sortSprites();
	spritesMutex.lock();
	for (Sprite* s : sprites) {
		s->updatePosition(windowPosition);
		s->draw(window);
	}
	spritesMutex.unlock();
}
