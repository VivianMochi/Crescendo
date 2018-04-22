#pragma once

#include "state.hpp"
#include "bitmap_text.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

struct Particle {
	sf::Sprite sprite;
	sf::Vector2f velocity;
	float life;
};

class MenuState : public State {
public:
	MenuState();
	~MenuState();

	void init();
	void gotEvent(sf::Event event);
	void update(sf::Time elapsed);
	void render(sf::RenderWindow &window);

	void createParticle(sf::Vector2f position, sf::Vector2f velocity, float lifespan);

private:
	sf::Sprite background;
	sf::Sprite title;
	BitmapText text;
	std::vector<Particle*> particles;

	sf::Music music;

	float beatCounter = 0;
};
