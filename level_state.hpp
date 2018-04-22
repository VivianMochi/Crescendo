#pragma once

#include "state.hpp"
#include "bitmap_text.hpp"
#include "infiltrator.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

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
	void setVolume(float volume);
	void setSection(int section);

	int level = 0;
	int section = 1;
	float volume = 0;

	float pregameTimer = 2;
	int bpm = 130;
	float beatCounter = 0;
	int beat = 0;

	sf::Sprite elevator;
	BitmapText floorDisplay;
	sf::Sprite levelSprite;
	sf::Image levelMask;
	Infiltrator player;

	sf::Sprite hud;
	sf::Sprite ambientBar;
	sf::Sprite personalBar;

	sf::Sound elevatorSound;
	sf::Sound doorSound;
	std::vector<sf::Music*> music;
};

