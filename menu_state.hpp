#pragma once

#include "state.hpp"
#include "bitmap_text.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class MenuState : public State {
public:
	MenuState();
	~MenuState();

	void init();
	void gotEvent(sf::Event event);
	void update(sf::Time elapsed);
	void render(sf::RenderWindow &window);

private:
	sf::Sprite title;
	BitmapText text;
	sf::Music music;

	float beatCounter = 0;
};
