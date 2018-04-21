#pragma once

#include "state.hpp"
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

private:
	int level;
};

