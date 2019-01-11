#include "Sprite.h"

vec rotate(vec ref, double angle) {
    double c = cos(angle * 3.141592 / 180.0), s = sin(angle * 3.14 / 180.0);
    return vec(ref.x * c - ref.y * s, ref.x * s + ref.y * c);
}

Texture::Texture(std::string path) : sf::Texture() {
    if(!this->loadFromFile(path)) {
        std::cerr << "Error loading texture from " << path << std::endl;
    }
    else {
        std::cout << "Loaded texture " << path << std::endl;
    }
}

SubSprite::SubSprite(vec parentPivot, vec childPivot, std::shared_ptr<Texture> texture, sf::IntRect rect) : parentPivot(parentPivot), childPivot(childPivot), angle(0) {
    this->spr.setTexture(*texture);
    this->spr.setTextureRect(rect);
    this->spr.setOrigin(childPivot);
}

std::shared_ptr<SubSprite> SubSprite::addSubSprite(vec parentPivot, vec childPivot, std::shared_ptr<Texture> texture, sf::IntRect rect, double angle) {
    auto s = std::make_shared<SubSprite>(parentPivot, childPivot, texture, rect);
    s->angle = angle;
    this->children.push_back(s);
    return s;
}

void SubSprite::draw(sf::RenderWindow* window, vec pos, double angle) {
    this->spr.setPosition(pos);
    this->spr.setRotation(angle + this->angle);
    // Draw children
    for(auto const i : this->children) {
        i->draw(window, pos - rotate(this->childPivot, angle + this->angle) + rotate(i->parentPivot, angle + this->angle), angle + this->angle);
    }
    // Draw self
    window->draw(this->spr);
}

double SubSprite::getAngle() {
    return this->angle;
}

void SubSprite::setAngle(double angle) {
    this->angle = angle;
}

Pose::Pose(vec rootPos, std::shared_ptr<Texture> texture, sf::IntRect rootRect) : root(std::make_shared<SubSprite>(rootPos, vec(0, 0), texture, rootRect)) {
    
}

void Pose::draw(sf::RenderWindow* window, vec pos) {
    this->root->draw(window, pos, 0);
}

std::shared_ptr<SubSprite> Pose::getRoot() {
    return this->root;
}

Sprite::Sprite(vec pos, std::shared_ptr<Texture> texture, sf::IntRect rootRect) : direction(Game::Direction::South) {
    this->poses.south = std::make_shared<Pose>(pos, texture, rootRect);
    this->poses.north = this->poses.south;
    this->poses.east = this->poses.south;
    this->poses.west = this->poses.south;
}

std::shared_ptr<Pose> Sprite::getPose(Game::Direction dir) {
    switch(dir) {
        case Game::Direction::North:
            return this->poses.north;
        case Game::Direction::East:
        case Game::Direction::West:
        default: 
            return this->poses.south;
    }
}

std::shared_ptr<Pose> Sprite::getCurrentPose() {
    return this->getPose(this->direction);
}

void Sprite::addPose(Game::Direction dir, vec pos, std::shared_ptr<Texture> texture, sf::IntRect rootRect) {
    switch(dir) {
        case Game::Direction::North:
            this->poses.north = std::make_shared<Pose>(pos, texture, rootRect);
            break;
        case Game::Direction::East:
            this->poses.east = std::make_shared<Pose>(pos, texture, rootRect);
            break;
        case Game::Direction::West:
            this->poses.west = std::make_shared<Pose>(pos, texture, rootRect);
            break;
        default:
            break;
    }
}

void Sprite::setDirection(Game::Direction dir) {
    this->direction = dir;
}

HumanoidFrame initHumanoidFrame(sf::Time time) {
    HumanoidData<double> angles;
    HumanoidData<double> scales;
    angles.head = NAN;
    angles.body = NAN;
    angles.arm1r = NAN;
    angles.arm2r = NAN;
    angles.arm1l = NAN;
    angles.arm2l = NAN;
    angles.leg1r = NAN;
    angles.leg2r = NAN;
    angles.leg1l = NAN;
    angles.leg2l = NAN;
    scales.head = 1;
    scales.body = 1;
    scales.arm1r = 1;
    scales.arm2r = 1;
    scales.arm1l = 1;
    scales.arm2l = 1;
    scales.leg1r = 1;
    scales.leg2r = 1;
    scales.leg1l = 1;
    scales.leg2l = 1;
    HumanoidFrame frame;
    frame.angles = angles;
    frame.scales = scales;
    frame.duration = time;
    return frame;
}

Humanoid::Humanoid(vec pos, std::shared_ptr<Texture> tex) : Sprite(pos, tex, sf::IntRect(96, 72, 64, 88)), desiredPosition(initHumanoidFrame(sf::Time::Zero).angles), animationCounter(sf::Time::Zero) {
    this->skeleton.south.body = this->getPose(Game::Direction::South)->getRoot();
    this->skeleton.south.head = this->getPose(Game::Direction::South)->getRoot()->addSubSprite(    vec(32, 5),   vec(32, 59), tex, sf::IntRect(96, 0, 64, 64),   0);
    this->skeleton.south.arm1r = this->getPose(Game::Direction::South)->getRoot()->addSubSprite(   vec(56, 16),  vec(5, 8),   tex, sf::IntRect(168, 80, 40, 16), 20);
    this->skeleton.south.arm2r = this->skeleton.south.arm1r->addSubSprite(                         vec(35, 8),   vec(5, 8),   tex, sf::IntRect(216, 80, 40, 16), 10);
    this->skeleton.south.arm1l = this->getPose(Game::Direction::South)->getRoot()->addSubSprite(   vec(8, 16),   vec(35, 8),  tex, sf::IntRect(48, 80, 40, 16),  -20);
    this->skeleton.south.arm2l = this->skeleton.south.arm1l->addSubSprite(                         vec(5, 8),    vec(35, 8),  tex, sf::IntRect(0, 80, 40, 16),   -10);
    this->skeleton.south.leg1r = this->getPose(Game::Direction::South)->getRoot()->addSubSprite(   vec(50, 70),  vec(8, 5),   tex, sf::IntRect(144, 168, 16, 40), -10);
    this->skeleton.south.leg2r = this->skeleton.south.leg1r->addSubSprite(                         vec(8, 35),   vec(8, 5),   tex, sf::IntRect(144, 216, 16, 40), 5);
    this->skeleton.south.leg1l = this->getPose(Game::Direction::South)->getRoot()->addSubSprite(   vec(14, 70),  vec(8, 5),   tex, sf::IntRect(96, 168, 16, 40), 10);
    this->skeleton.south.leg2l = this->skeleton.south.leg1l->addSubSprite(                         vec(8, 35),   vec(8, 5),   tex, sf::IntRect(96, 216, 16, 40), -5);
}

void Humanoid::updateSkeleton(sf::Time elapsed) {
    this->updateMember(this->desiredPosition.head, this->getCurrentSkeleton().head, elapsed);
    this->updateMember(this->desiredPosition.body, this->getCurrentSkeleton().body, elapsed);
    this->updateMember(this->desiredPosition.arm1r, this->getCurrentSkeleton().arm1r, elapsed);
    this->updateMember(this->desiredPosition.arm2r, this->getCurrentSkeleton().arm2r, elapsed);
    this->updateMember(this->desiredPosition.arm1l, this->getCurrentSkeleton().arm1l, elapsed);
    this->updateMember(this->desiredPosition.arm2l, this->getCurrentSkeleton().arm2l, elapsed);
    this->updateMember(this->desiredPosition.leg1r, this->getCurrentSkeleton().leg1r, elapsed);
    this->updateMember(this->desiredPosition.leg2r, this->getCurrentSkeleton().leg2r, elapsed);
    this->updateMember(this->desiredPosition.leg1l, this->getCurrentSkeleton().leg1l, elapsed);
    this->updateMember(this->desiredPosition.leg2l, this->getCurrentSkeleton().leg2l, elapsed);
}

HumanoidData<std::shared_ptr<SubSprite>>& Humanoid::getSkeleton(Game::Direction dir) {
    switch(dir) {
        case Game::Direction::North:
            return this->skeleton.north;
        case Game::Direction::East:
            return this->skeleton.east;
        case Game::Direction::West:
            return this->skeleton.west;
        default:
            return this->skeleton.south;
    }
}

HumanoidData<std::shared_ptr<SubSprite>>& Humanoid::getCurrentSkeleton() {
    return this->getSkeleton(this->direction);
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
    this->desiredPosition.head = frame.angles.head;
    this->desiredPosition.body = frame.angles.body;
    this->desiredPosition.arm1r = frame.angles.arm1r;
    this->desiredPosition.arm2r = frame.angles.arm2r;
    this->desiredPosition.arm1l = frame.angles.arm1l;
    this->desiredPosition.arm2l = frame.angles.arm2l;
    this->desiredPosition.leg1r = frame.angles.leg1r;
    this->desiredPosition.leg2r = frame.angles.leg2r;
    this->desiredPosition.leg1l = frame.angles.leg1l;
    this->desiredPosition.leg2l = frame.angles.leg2l;
    this->updateSkeleton(elapsed);
}

void Humanoid::addPose(Game::Direction dir, vec pos, std::shared_ptr<Texture> texture) {
    Sprite::addPose(dir, pos, texture, sf::IntRect(96, 72, 64, 88));
    switch(dir) {
        case Game::Direction::North:
            this->getSkeleton(dir).body = this->getPose(dir)->getRoot();
            this->getSkeleton(dir).head = this->getPose(dir)->getRoot()->addSubSprite(    vec(32, 5),   vec(32, 59), texture, sf::IntRect(96, 0, 64, 64),   0);
            this->getSkeleton(dir).arm1r = this->getPose(dir)->getRoot()->addSubSprite(   vec(56, 16),  vec(5, 8),   texture, sf::IntRect(168, 80, 40, 16), 20);
            this->getSkeleton(dir).arm2r = this->getSkeleton(dir).arm1r->addSubSprite(    vec(35, 8),   vec(5, 8),   texture, sf::IntRect(216, 80, 40, 16), 10);
            this->getSkeleton(dir).arm1l = this->getPose(dir)->getRoot()->addSubSprite(   vec(8, 16),   vec(35, 8),  texture, sf::IntRect(48, 80, 40, 16),  -20);
            this->getSkeleton(dir).arm2l = this->getSkeleton(dir).arm1l->addSubSprite(    vec(5, 8),    vec(35, 8),  texture, sf::IntRect(0, 80, 40, 16),   -10);
            this->getSkeleton(dir).leg1r = this->getPose(dir)->getRoot()->addSubSprite(   vec(50, 70),  vec(8, 5),   texture, sf::IntRect(144, 168, 16, 40), -10);
            this->getSkeleton(dir).leg2r = this->getSkeleton(dir).leg1r->addSubSprite(    vec(8, 35),   vec(8, 5),   texture, sf::IntRect(144, 216, 16, 40), 5);
            this->getSkeleton(dir).leg1l = this->getPose(dir)->getRoot()->addSubSprite(   vec(14, 70),  vec(8, 5),   texture, sf::IntRect(96, 168, 16, 40), 10);
            this->getSkeleton(dir).leg2l = this->getSkeleton(dir).leg1l->addSubSprite(    vec(8, 35),   vec(8, 5),   texture, sf::IntRect(96, 216, 16, 40), -5);
            break;
        case Game::Direction::East:
        case Game::Direction::West:
        default:
            break;
    }
}

AssetsCollector::AssetsCollector() {}

std::shared_ptr<Texture> AssetsCollector::addTexture(std::string path) {
    auto s = std::make_shared<Texture>(path);
    this->textures.push_back(s);
    return s;
}
