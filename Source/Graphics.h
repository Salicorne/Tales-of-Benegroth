#pragma once
#include "tools.h"

class Texture : public sf::Texture {
	protected: 
		game_id id;
		bool loaded;

	public:
		Texture(std::string path, game_id id);
		game_id getId();
		bool isLoaded();
};

class Sprite : public sf::Sprite {
	protected: 
		game_id id;
		bool loaded;
		int layer;
		vec2f posInWorld;
		sf::IntRect textureRect;
		int feetOffset;
		bool mustBlur;

	public:
		Sprite(Texture& texture, vec2f posInWorld, game_id id, float feetOffset);
		game_id getId();
		int getLayer() const;
		void setLayer(int layer);
		void updatePosition(vec2f windowPos);
		vec2f getFeetPos() const;
		vec2f getHeadPos() const;
		vec2f getPosInWorld() const;
		int getFeetOffset() const;
		void blurs(bool b);
		bool blurs();

		virtual void blur(bool b);
		virtual void draw(sf::RenderTarget& window, sf::Shader* shader);
		virtual void setPosInWorld(vec2f pos);
		virtual void move(vec2f delta);

		virtual void animate(sf::Time elapsed);
		virtual void setAnimation(Game::Animation a);
};

class TreeSprite : public Sprite {
	protected:
		sf::Sprite leaves;
		int offset;

	public:
		TreeSprite(Texture& trunk, Texture& top, int offset, vec2f posInWorld, game_id id, float feetOffset);
		virtual void draw(sf::RenderTarget& window, sf::Shader* shader);
		virtual void blur(bool b);
};

class RepeatedSprite : public Sprite {
	public:
		RepeatedSprite(Texture& texture, int w, int h, vec2f posInWorld, game_id id, float feetOffset);
};

class AnimatedSprite : public Sprite {
	protected:
		sf::Time animationDuration;
		sf::Time animationCounter;

	public: 
		AnimatedSprite(Texture& texture, sf::IntRect rect, sf::Time duration, vec2f posInWorld, game_id id, float feetOffset);
		virtual void setAnimation(Game::Animation a);
		virtual void animate(sf::Time elapsed);
		void animateStep();
};

class AdditionalSprite : public AnimatedSprite {
	public:
		AdditionalSprite(Texture& texture, sf::IntRect rect, sf::Time duration, game_id id);
		virtual void draw(sf::RenderTarget& window, sf::Shader* shader);
		virtual void animate(sf::Time elapsed);
};

class SpriteSet : public AnimatedSprite {
	protected: 
		std::vector<AdditionalSprite*> sprites;

	public:
		SpriteSet(Texture& texture, sf::IntRect rect, sf::Time duration, vec2f posInWorld, game_id id, float feetOffset);
		void addAdditionalSprite(AdditionalSprite& spr);
		virtual void draw(sf::RenderTarget& window, sf::Shader* shader);
		virtual void setAnimation(Game::Animation a);
		virtual void move(vec2f delta);
		virtual void setPosInWorld(vec2f pos);
		virtual void animate(sf::Time elapsed);
		virtual void blur(bool b);
};