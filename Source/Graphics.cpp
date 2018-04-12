#include "Graphics.h"



Texture::Texture(std::string path, id_t id) : id(id) {
	loaded = path != "" && this->loadFromFile(path);
	if (!loaded) {
		if (path != "") { std::cerr << "Error loading texture at " << path << std::endl; }
		this->create(50, 50);
		sf::Image img;
		img.create(50, 50, sf::Color::Yellow);
		for (int x = 1; x < 50; x+=2) {
			for (int y = 1; y < 50; y+=2) {
				img.setPixel(x, y, sf::Color::Red);
			}
		}
		this->update(img);
	}
}

id_t Texture::getId() {
	return id;
}

bool Texture::isLoaded() {
	return loaded;
}

Sprite::Sprite(Texture& texture, vec2f posInWorld, id_t id, float feetOffset) : id(id), layer(0), posInWorld(posInWorld) {
	this->setTexture(texture);
	loaded = texture.isLoaded();
	textureRect = sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y);
	this->feetOffset = textureRect.height * feetOffset;
}

id_t Sprite::getId() {
	return id;
}

void Sprite::animate(sf::Time elapsed) {}

void Sprite::setAnimation(Game::Animation a) {}

void Sprite::draw(sf::RenderTarget& window) {
	window.draw(*this);
}

int Sprite::getLayer() const {
	return layer;
}

void Sprite::setLayer(int layer) {
	this->layer = layer;
}

void Sprite::updatePosition(vec2f windowPos) {
	setPosition(posInWorld.x - windowPos.x, posInWorld.y - windowPos.y);
}

vec2f Sprite::getFeetPos() const {
	return vec2f(posInWorld.x + textureRect.width/2, posInWorld.y + textureRect.height - feetOffset);
}

vec2f Sprite::getHeadPos() const {
	return vec2f(posInWorld.x + textureRect.width/2, posInWorld.y);
}

vec2f Sprite::getPosInWorld() const {
	return posInWorld;
}

void Sprite::blur(bool b) {
	setColor(sf::Color(255, 255, 255, 255*b?0.5:1));
}

void Sprite::setPosInWorld(vec2f pos) {
	posInWorld = pos;
}

void Sprite::move(vec2f delta) {
	this->posInWorld += delta;
}

AnimatedSprite::AnimatedSprite(Texture& texture, sf::IntRect rect, sf::Time duration, vec2f posInWorld, id_t id, float feetOffset) : animationDuration(duration), Sprite(texture, posInWorld, id, feetOffset) {
	setTextureRect(rect);
	textureRect = rect;
	animationCounter = sf::Time::Zero;
	setAnimation(Game::Animation::IdleS);
}

void AnimatedSprite::animateStep() {
	sf::IntRect rect = getTextureRect();
	setTextureRect(sf::IntRect((unsigned int(rect.left + rect.width*2) > getTexture()->getSize().x) ? 0 : rect.left + rect.width, rect.top, rect.width, rect.height));
}

void AnimatedSprite::setAnimation(Game::Animation a) {
	sf::IntRect rect = getTextureRect();
	setTextureRect(sf::IntRect(rect.left, rect.height * a, rect.width, rect.height));
}

void AnimatedSprite::animate(sf::Time elapsed) {
	if (!loaded) { return; }
	animationCounter += elapsed;
	while (animationCounter > animationDuration) {
		animationCounter -= animationDuration;
		animateStep();
	}
}

RepeatedSprite::RepeatedSprite(Texture& texture, int w, int h, vec2f posInWorld, id_t id, float feetOffset) : Sprite(texture, posInWorld, id, feetOffset) {
	texture.setRepeated(true);
	setTextureRect(sf::IntRect(0, 0, w, h));
}

AdditionalSprite::AdditionalSprite(Texture& texture, sf::IntRect rect, sf::Time duration, id_t id) : AnimatedSprite(texture, rect, duration, vec2f(0,0), id, 1) {}

void AdditionalSprite::draw(sf::RenderTarget & window) {}

void AdditionalSprite::animate(sf::Time elapsed) {}

SpriteSet::SpriteSet(Texture& texture, sf::IntRect rect, sf::Time duration, vec2f posInWorld, id_t id, float feetOffset) : AnimatedSprite(texture, rect, duration, posInWorld, id, feetOffset) {}

void SpriteSet::addAdditionalSprite(AdditionalSprite& spr) {
	sprites.push_back(&spr);
	spr.setPosInWorld(getPosInWorld());
}

void SpriteSet::draw(sf::RenderTarget& window) {
	window.draw(*this);
	for (const AdditionalSprite* spr : sprites) {
		window.draw(*spr);
	}
}

void SpriteSet::setAnimation(Game::Animation a) {
	sf::IntRect rect = getTextureRect();
	setTextureRect(sf::IntRect(rect.left, rect.height * a, rect.width, rect.height));

	for (AdditionalSprite* s : sprites) {
		sf::IntRect rect = s->getTextureRect();
		s->setTextureRect(sf::IntRect(rect.left, rect.height * a, rect.width, rect.height));
	}
}

void SpriteSet::move(vec2f delta) {
	posInWorld += delta;
	for (AdditionalSprite* spr : sprites) {
		spr->move(delta);
	}
}

void SpriteSet::setPosInWorld(vec2f pos) {
	posInWorld = pos;
	for (AdditionalSprite* spr : sprites) {
		spr->setPosInWorld(pos);
	}
}

void SpriteSet::animate(sf::Time elapsed) {
	if (!loaded) { return; }
	animationCounter += elapsed;
	while (animationCounter > animationDuration) {
		animationCounter -= animationDuration;
		animateStep();
		for (AdditionalSprite* spr : sprites) {
			spr->animateStep();
		}
	}
}

TreeSprite::TreeSprite(Texture& trunk, Texture& top, int offset, vec2f posInWorld, id_t id, float feetOffset) : Sprite(trunk, posInWorld, id, feetOffset), offset(offset) {
	leaves.setTexture(top);
}

void TreeSprite::draw(sf::RenderTarget& window) {
	window.draw(*this);
	leaves.setPosition(getPosition() - vec2f(0, offset));
	window.draw(leaves);
}

