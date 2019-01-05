#include "Sprite.h"

vec rotate(vec ref, double angle) {
    double c = cos(angle * 3.141592 / 180.0), s = sin(angle * 3.14 / 180.0);
    return vec(ref.x * c - ref.y * s, ref.x * s + ref.y * c);
}

SubSprite::SubSprite(vec parentPivot, vec childPivot, sf::Color color, vec size) : parentPivot(parentPivot), childPivot(childPivot), angle(0) {
    this->shape = sf::RectangleShape(size);
    this->shape.setFillColor(color);
    this->shape.setOrigin(childPivot);
}

std::shared_ptr<SubSprite> SubSprite::addSubSprite(vec parentPivot, vec childPivot, sf::Color c, vec size, double angle) {
    auto s = std::make_shared<SubSprite>(parentPivot, childPivot, c, size);
    s->angle = angle;
    this->children.push_back(s);
    return s;
}

void SubSprite::draw(sf::RenderWindow* window, vec pos, double angle) {
    this->shape.setPosition(pos);
    this->shape.setRotation(angle + this->angle);
    // Draw children
    for(auto const i : this->children) {
        i->draw(window, pos - rotate(this->childPivot, angle + this->angle) + rotate(i->parentPivot, angle + this->angle), angle + this->angle);
    }
    // Draw self
    window->draw(this->shape);
}

void SubSprite::animate(double delta) { this->angle+=delta; }

Sprite::Sprite(vec rootPos, sf::Color color, vec size) : root(std::make_shared<SubSprite>(rootPos, vec(0, 0), color, size)) {
    std::cout << "hi" << std::endl;
}

void Sprite::draw(sf::RenderWindow* window, vec pos) {
    this->root->draw(window, pos, 0);
}

std::shared_ptr<SubSprite> Sprite::getRoot() {
    return this->root;
}

Humanoid::Humanoid(vec pos) : Sprite(pos, sf::Color::Green, vec(80, 120)) {
    auto head = this->root->addSubSprite(   vec(40, 5),  vec(30, 60), sf::Color::Yellow,  vec(60, 60), 0);
    
    auto arm1r = this->root->addSubSprite(  vec(75, 20), vec(5, 10),  sf::Color::Magenta, vec(50, 20), 20);
    auto arm2r =    arm1r->addSubSprite(    vec(45, 10), vec(5, 10),  sf::Color::Red,     vec(50, 20), 10);
    
    auto arm1l = this->root->addSubSprite(  vec(5, 20),  vec(45, 10), sf::Color::Magenta, vec(50, 20), -20);
    auto arm2l =    arm1l->addSubSprite(    vec(5, 10), vec(45, 10), sf::Color::Red,     vec(50, 20), -10);
    
    auto leg1r = this->root->addSubSprite(  vec(60, 115),  vec(10, 5), sf::Color::Magenta, vec(20, 50), -10);
    auto leg2r = leg1r->addSubSprite(  vec(10, 45),  vec(10, 5), sf::Color::Red, vec(20, 50), 5);

    auto leg1l = this->root->addSubSprite(  vec(20, 115),  vec(10, 5), sf::Color::Magenta, vec(20, 50), 10);
    auto leg2l = leg1l->addSubSprite(  vec(10, 45),  vec(10, 5), sf::Color::Red, vec(20, 50), -5);
}