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

double SubSprite::getAngle() {
    return this->angle;
}

void SubSprite::setAngle(double angle) {
    this->angle = angle;
}

Sprite::Sprite(vec rootPos, sf::Color color, vec size) : root(std::make_shared<SubSprite>(rootPos, vec(0, 0), color, size)) {
    
}

void Sprite::draw(sf::RenderWindow* window, vec pos) {
    this->root->draw(window, pos, 0);
}

std::shared_ptr<SubSprite> Sprite::getRoot() {
    return this->root;
}

HumanoidFrame initHumanoidFrame(sf::Time time) {
    HumanoidData<double> image;
    image.head = NAN;
    image.body = NAN;
    image.arm1r = NAN;
    image.arm2r = NAN;
    image.arm1l = NAN;
    image.arm2l = NAN;
    image.leg1r = NAN;
    image.leg2r = NAN;
    image.leg1l = NAN;
    image.leg2l = NAN;
    HumanoidFrame frame;
    frame.image = image;
    frame.duration = time;
    return frame;
}

Humanoid::Humanoid(vec pos) : Sprite(pos, sf::Color::Green, vec(80, 120)), desiredPosition(initHumanoidFrame(sf::Time::Zero).image), animationCounter(sf::Time::Zero) {
    this->skeleton.body = this->root;
    this->skeleton.head = this->root->addSubSprite(   vec(40, 5),   vec(30, 60), sf::Color::Yellow,  vec(60, 60), 0);
    this->skeleton.arm1r = this->root->addSubSprite(  vec(75, 20),  vec(5, 10),  sf::Color::Magenta, vec(50, 20), 20);
    this->skeleton.arm2r = skeleton.arm1r->addSubSprite(       vec(45, 10),  vec(5, 10),  sf::Color::Red,     vec(50, 20), 10);
    this->skeleton.arm1l = this->root->addSubSprite(  vec(5, 20),   vec(45, 10), sf::Color::Magenta, vec(50, 20), -20);
    this->skeleton.arm2l = skeleton.arm1l->addSubSprite(       vec(5, 10),   vec(45, 10), sf::Color::Red,     vec(50, 20), -10);
    this->skeleton.leg1r = this->root->addSubSprite(  vec(60, 115), vec(10, 5),  sf::Color::Magenta, vec(20, 50), -10);
    this->skeleton.leg2r = skeleton.leg1r->addSubSprite(       vec(10, 45),  vec(10, 5),  sf::Color::Red,     vec(20, 50), 5);
    this->skeleton.leg1l = this->root->addSubSprite(  vec(20, 115), vec(10, 5),  sf::Color::Magenta, vec(20, 50), 10);
    this->skeleton.leg2l = skeleton.leg1l->addSubSprite(       vec(10, 45),  vec(10, 5),  sf::Color::Red,     vec(20, 50), -5);
}

void Humanoid::updateSkeleton(sf::Time elapsed) {
    this->updateMember(this->desiredPosition.head, this->skeleton.head, elapsed);
    this->updateMember(this->desiredPosition.body, this->skeleton.body, elapsed);
    this->updateMember(this->desiredPosition.arm1r, this->skeleton.arm1r, elapsed);
    this->updateMember(this->desiredPosition.arm2r, this->skeleton.arm2r, elapsed);
    this->updateMember(this->desiredPosition.arm1l, this->skeleton.arm1l, elapsed);
    this->updateMember(this->desiredPosition.arm2l, this->skeleton.arm2l, elapsed);
    this->updateMember(this->desiredPosition.leg1r, this->skeleton.leg1r, elapsed);
    this->updateMember(this->desiredPosition.leg2r, this->skeleton.leg2r, elapsed);
    this->updateMember(this->desiredPosition.leg1l, this->skeleton.leg1l, elapsed);
    this->updateMember(this->desiredPosition.leg2l, this->skeleton.leg2l, elapsed);
}

void Humanoid::updateMember(double& desired, std::shared_ptr<SubSprite> ss, sf::Time elapsed) {
    if(!std::isnan(desired)) {
        double delta = (desired - ss->getAngle()) * elapsed.asSeconds() * 15;
        if(fabs(delta) > 0.00001) { 
            ss->setAngle(ss->getAngle() + delta);
        }
    }
}

void Humanoid::setAnimation(HumanoidAnimation a) {
    this->animation = a;
}

void Humanoid::animate(sf::Time elapsed) {
    if(animation.empty()) { return; }
    this->animationCounter += elapsed;
    HumanoidFrame frame = this->animation.front();
    // Update current frame
    if(this->animationCounter >= frame.duration) {
        this->animation.push(frame);
        this->animation.pop();
        this->animationCounter -= frame.duration;
        frame = this->animation.front();
    }
    // Apply frame
    this->desiredPosition.head = frame.image.head;
    this->desiredPosition.body = frame.image.body;
    this->desiredPosition.arm1r = frame.image.arm1r;
    this->desiredPosition.arm2r = frame.image.arm2r;
    this->desiredPosition.arm1l = frame.image.arm1l;
    this->desiredPosition.arm2l = frame.image.arm2l;
    this->desiredPosition.leg1r = frame.image.leg1r;
    this->desiredPosition.leg2r = frame.image.leg2r;
    this->desiredPosition.leg1l = frame.image.leg1l;
    this->desiredPosition.leg2l = frame.image.leg2l;
    this->updateSkeleton(elapsed);
}