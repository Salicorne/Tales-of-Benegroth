#include "Sprite.h"

vec rotate(vec ref, double angle) {
    double c = cos(angle * 3.141592 / 180.0), s = sin(angle * 3.14 / 180.0);
    return vec(ref.x * c - ref.y * s, ref.x * s + ref.y * c);
}

SubSprite::SubSprite(vec parentPivot, vec childPivot, sf::Color color) : parentPivot(parentPivot), childPivot(childPivot), angle(0) {
    this->shape = sf::RectangleShape(vec(200, 50));
    this->shape.setFillColor(color);
    this->shape.setOrigin(childPivot);
}

std::shared_ptr<SubSprite> SubSprite::addSubSprite(vec parentPivot, vec childPivot, sf::Color c, double angle) {
    auto s = std::make_shared<SubSprite>(parentPivot, childPivot, c);
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

Sprite::Sprite(vec rootPos) : root(std::make_shared<SubSprite>(rootPos, vec(0, 0), sf::Color::Blue)) {
    std::cout << "hi" << std::endl;
}

void Sprite::draw(sf::RenderWindow* window, vec pos) {
    this->root->draw(window, pos, 0);
}

std::shared_ptr<SubSprite> Sprite::getRoot() {
    return this->root;
}