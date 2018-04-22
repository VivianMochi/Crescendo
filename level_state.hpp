#pragma once

#include "state.hpp"
#include "bitmap_text.hpp"
#include "infiltrator.hpp"
#include "lever.hpp"
#include "beat_block.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class LevelState : public State {
public:
	LevelState(int level, bool skipIntro = false);
	~LevelState();

	void init();
	void gotEvent(sf::Event event);
	void update(sf::Time elapsed);
	void render(sf::RenderWindow &window);

	bool checkCollision(sf::Vector2f position, int width, int height);
	bool isMetal(sf::Vector2f position);

private:
	void setupLevel();
	void calculateVolume();

	void setVolume(float volume);
	void setSection(int section);

	int level = 0;
	int section = 1;
	float volume = 0;
	float alert = 0;
	int gameState = 0;
	float gameTimer = 2;

	int bpm = 130;
	float beatCounter = 0;
	int beat = 0;

	sf::Sprite elevator;
	BitmapText floorDisplay;
	sf::Sprite levelSprite;
	sf::Image levelMask;
	Infiltrator player;
	std::vector<Lever*> levers;
	std::vector<BeatBlock*> blocks;

	sf::Sprite hud;
	sf::Sprite ambientBar;
	sf::Sprite personalBar;
	sf::Sprite alertOverlay;

	sf::Sound elevatorSound;
	sf::Sound doorSound;
	sf::Sound alertSound;
	std::vector<sf::Music*> music;
};

