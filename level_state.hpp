#pragma once

#include "state.hpp"
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
	void emitSound(sf::Vector2f position, int volume);

private:
	int level = 0;
	float volume = 0;

	sf::Music music;
	Infiltrator player;
};

