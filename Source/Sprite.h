#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>

typedef sf::Vector2f vec;

vec rotate(vec ref, double angle);

class SubSprite {
    protected:
        std::vector<std::shared_ptr<SubSprite>> children;
        vec parentPivot;
        vec childPivot;
        double angle;
        sf::Color color;
        sf::RectangleShape shape;

    public: 
        SubSprite(vec parentPivot, vec childPivot, sf::Color color, vec size);
        std::shared_ptr<SubSprite> addSubSprite(vec parentPivot, vec childPivot, sf::Color c, vec size, double angle);
        void draw(sf::RenderWindow* window, vec pos, double angle);
        void animate(double delta);
};

class Sprite {
    protected:
        std::shared_ptr<SubSprite> root;
        vec pos;
    public:
        Sprite(vec pos, sf::Color color, vec size);
        void draw(sf::RenderWindow* window, vec pos);
        std::shared_ptr<SubSprite> getRoot();
};

class Humanoid : public Sprite {
    public: 
        Humanoid(vec pos);
};