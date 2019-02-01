#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <queue>
#include "game.h"

typedef sf::Vector2f vec;

vec rotate(vec ref, double angle);
vec scaleY(vec ref, double yFactor);

class Texture : public sf::Texture {
    public:
        Texture(std::string path);
};

class SubSprite {
    protected:
        std::vector<std::shared_ptr<SubSprite>> children;
        vec parentPivot;
        vec childPivot;
        double angle;
        double scale;
        sf::Sprite spr;

    public: 
        SubSprite(vec parentPivot, vec childPivot, std::shared_ptr<Texture> texture, sf::IntRect rect);
        std::shared_ptr<SubSprite> addSubSprite(vec parentPivot, vec childPivot, std::shared_ptr<Texture> texture, sf::IntRect rect, double angle);
        void draw(sf::RenderWindow* window, vec pos, double angle);
        double getAngle();
        void setAngle(double angle);
        double getScale();
        void setScale(double scale);
};

class Pose {
    protected:
        std::shared_ptr<SubSprite> root;
        vec pos;

    public:
        Pose(vec pos, std::shared_ptr<Texture> texture, sf::IntRect rootRect);
        void draw(sf::RenderWindow* window, vec pos);
        std::shared_ptr<SubSprite> getRoot();
};

template <typename T> struct Oriented {
    T north;
    T south;
    T east;
    T west;
};

class Sprite {
    protected:
        Oriented<std::shared_ptr<Pose>> poses;
        Game::Direction direction;

    public: 
        Sprite(vec pos, std::shared_ptr<Texture> texture, sf::IntRect rootRect);
        std::shared_ptr<Pose> getPose(Game::Direction dir);
        std::shared_ptr<Pose> getCurrentPose();
        void addPose(Game::Direction dir, vec pos, std::shared_ptr<Texture> texture, sf::IntRect rootRect);
        void setDirection(Game::Direction dir);
};

template <typename T> struct HumanoidData {
    T body;
    T head;
    T arm1r; 
    T arm2r;
    T arm1l; 
    T arm2l;
    T leg1r;
    T leg2r;
    T leg1l;
    T leg2l; 
};

template <typename T> struct Frame {
    T angles;
    T scales;
    sf::Time duration;
};

typedef Frame<HumanoidData<double>> HumanoidFrame;

HumanoidFrame initHumanoidFrame(sf::Time time);

typedef std::queue<HumanoidFrame> HumanoidAnimation;

class Humanoid : public Sprite {
    protected:
        Oriented<HumanoidData<std::shared_ptr<SubSprite>>> skeleton;
        HumanoidData<double> desiredAngles;
        HumanoidData<double> desiredScales;
        sf::Time animationCounter;
        HumanoidAnimation animation;
        void updateSkeleton(sf::Time elapsed);
        HumanoidData<std::shared_ptr<SubSprite>>& getCurrentSkeleton();
        HumanoidData<std::shared_ptr<SubSprite>>& getSkeleton(Game::Direction dir);
        void updateMember(double& desiredAngle, double& desiredScale, std::shared_ptr<SubSprite> ss, sf::Time elapsed);

    public: 
        Humanoid(vec pos, std::shared_ptr<Texture> tex);
        void setAnimation(HumanoidAnimation a);
        void animate(sf::Time elapsed);
        void addPose(Game::Direction dir, vec pos, std::shared_ptr<Texture> texture);
};

class AssetsCollector {
    protected:
        std::vector<std::shared_ptr<Texture>> textures;
        std::vector<std::shared_ptr<Sprite>> sprites;

    public:
        AssetsCollector();
        std::shared_ptr<Texture> addTexture(std::string path);
};
