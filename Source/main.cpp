#include "Sprite.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 640), "Demo app");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

	HumanoidAnimation animation;
	HumanoidFrame frame1 = initHumanoidFrame(sf::milliseconds(500));
	frame1.image.arm1l = 40;
	frame1.image.arm2l = 50;
	frame1.image.head = -10;
	HumanoidFrame frame2 = initHumanoidFrame(sf::milliseconds(500));
	frame2.image.arm1l = -20;
	frame2.image.arm2l = -30;
	frame2.image.head = 10;
	animation.push(frame1);
	animation.push(frame2);

	AssetsCollector mgr;
	auto txtr = mgr.addTexture("Assets/Humanoid.png");
	Humanoid h(vec(0, 0), txtr);
	h.setAnimation(animation);

	sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed) {
                window.close();
			}
        }
		sf::Time elapsed = clock.restart();

		h.animate(elapsed);

        window.clear();
        window.draw(shape);
		h.draw(&window, vec(300, 300));
        window.display();
    }

    return 0;
}

