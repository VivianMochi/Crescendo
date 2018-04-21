#pragma once

#include "state.hpp"
#include "bitmap_text.hpp"
#include "infiltrator.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class LevelState : public State {
public:
	LevelState(int level);
	~LevelState();

	void init();
	void gotEvent(sf::Event event);
	void update(sf::Time elapsed);
	void render(sf::RenderWindow &window);

	bool checkCollision(sf::Vector2f position, int width, int height);
	bool isMetal(sf::Vector2f position);

private:
	int level = 0;
	float volume = 1;

	sf::Sprite elevator;
	BitmapText floorDisplay;
	sf::Sprite levelSprite;
	sf::Image levelMask;
	Infiltrator player;

	sf::Sprite hud;
	sf::Sprite ambientBar;
	sf::Sprite personalBar;

	sf::Music music;
};

