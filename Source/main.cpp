#include "Sprite.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 640), "Demo app");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

	HumanoidAnimation animation;
	HumanoidFrame frame1 = initHumanoidFrame(sf::milliseconds(500));
	frame1.angles.arm1l = 40;
	frame1.angles.arm2l = 50;
	frame1.angles.head = -10;
	HumanoidFrame frame2 = initHumanoidFrame(sf::milliseconds(500));
	frame2.angles.arm1l = -20;
	frame2.angles.arm2l = -30;
	frame2.angles.head = 10;
	animation.push(frame1);
	animation.push(frame2);

	AssetsCollector mgr;
	auto txtr1 = mgr.addTexture("Assets/Humanoid.png");
	auto txtr2 = mgr.addTexture("Assets/Humanoid-1.png");
	Humanoid h(vec(0, 0), txtr1);
	h.addPose(Game::Direction::North, vec(0, 0), txtr2);
	h.setAnimation(animation);

	sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
			}
			if(event.type = sf::Event::KeyPressed) {
				if(event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				if(event.key.code == sf::Keyboard::P) {
					h.setDirection(Game::Direction::North);
				}
				if(event.key.code == sf::Keyboard::O) {
					h.setDirection(Game::Direction::South);
				}
			}
        }
		sf::Time elapsed = clock.restart();

		h.animate(elapsed);

        window.clear();
        window.draw(shape);
		h.getCurrentPose()->draw(&window, vec(300, 300));
        window.display();
    }

    return 0;
}

