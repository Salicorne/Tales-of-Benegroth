#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <queue>

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
        double getAngle();
        void setAngle(double angle);
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
    T image;
    sf::Time duration;
};

typedef Frame<HumanoidData<double>> HumanoidFrame;

HumanoidFrame initHumanoidFrame(sf::Time time);

typedef std::queue<HumanoidFrame> HumanoidAnimation;

class Humanoid : public Sprite {
    protected:
        HumanoidData<std::shared_ptr<SubSprite>> skeleton;
        HumanoidData<double> desiredPosition;
        sf::Time animationCounter;
        HumanoidAnimation animation;
        void updateSkeleton(sf::Time elapsed);
        void updateMember(double& desired, std::shared_ptr<SubSprite> ss, sf::Time elapsed);

    public: 
        Humanoid(vec pos);
        void setAnimation(HumanoidAnimation a);
        void animate(sf::Time elapsed);
};