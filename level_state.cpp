#include "level_state.hpp"

#include <iomanip>
#include <sstream>
#include <fstream>

LevelState::LevelState(int level) : level(level) {

}

LevelState::~LevelState() {

}

void LevelState::init() {
	elevator.setTexture(loadTexture("img/elevator.png"));
	elevator.setTextureRect(sf::IntRect(40, 0, 40, 135));

	floorDisplay.setTexture(loadTexture("img/font.png"));
	floorDisplay.setPosition(13, 98);
	floorDisplay.setColor(sf::Color(127, 0, 0));
	std::stringstream ss;
	ss << "lv" << level;
	floorDisplay.setText(ss.str());
	ss.str("");

	ss << "level/level" << level << ".png";
	levelSprite.setTexture(loadTexture(ss.str()));
	ss.str("");
	levelSprite.setPosition(40, 0);

	ss << "level/level" << level << "m.png";
	levelMask.loadFromFile(ss.str());
	ss.str("");

	player.setState(this);
	player.init();

	hud.setTexture(loadTexture("img/hud.png"));
	hud.setTextureRect(sf::IntRect(0, 0, 36, 91));

	ambientBar.setTexture(loadTexture("img/hud.png"));

	personalBar.setTexture(loadTexture("img/hud.png"));

	ss << "mus/level" << level << ".ogg";
	music.openFromFile(ss.str());
	music.setLoop(true);
	music.play();
}

void LevelState::gotEvent(sf::Event event) {

}

void LevelState::update(sf::Time elapsed) {
	player.update(elapsed);

	if (volume > 0) {
		float tempVolume = volume;
		if (tempVolume > 4) {
			tempVolume = 4;
		}
		int height = 69 * tempVolume / 4;
		ambientBar.setPosition(8, 18 + (69 - height));
		ambientBar.setTextureRect(sf::IntRect(36, 69 - height, 11, height));
	}
	if (player.getVolume() > 0) {
		float tempVolume = player.getVolume();
		if (tempVolume > 4) {
			tempVolume = 4;
		}
		int height = 69 * tempVolume / 4;
		personalBar.setPosition(21, 18 + (69 - height));
		personalBar.setTextureRect(sf::IntRect(47, 69 - height, 11, height));
	}
}

void LevelState::render(sf::RenderWindow &window) {
	window.draw(elevator);
	window.draw(floorDisplay);
	window.draw(levelSprite);

	window.draw(player);

	window.draw(hud);
	if (volume > 0) {
		window.draw(ambientBar);
	}
	if (player.getVolume()) {
		window.draw(personalBar);
	}
}

bool LevelState::checkCollision(sf::Vector2f position, int width, int height) {
	if (position.y + height > 125) {
		return true;
	}
	else if (position.y < 10) {
		return true;
	}
	else if (position.x + width > 230) {
		return true;
	}
	else if (position.x < 10) {
		return true;
	}
	else if (position.x < 40 && position.y < 95) {
		return true;
	}
	else if (position.x > 40 && position.x + width < 240 && position.y > 0 && position.y + height < 135) {
		if (levelMask.getPixel(position.x - 40, position.y).b == 255) {
			return true;
		}
		else if (levelMask.getPixel(position.x - 40 + width, position.y).b == 255) {
			return true;
		}
		else if (levelMask.getPixel(position.x - 40, position.y + height / 2).b == 255) {
			return true;
		}
		else if (levelMask.getPixel(position.x - 40 + width, position.y + height / 2).b == 255) {
			return true;
		}
		else if (levelMask.getPixel(position.x - 40, position.y + height).b == 255) {
			return true;
		}
		else if (levelMask.getPixel(position.x - 40 + width, position.y + height).b == 255) {
			return true;
		}
	}
	return false;
}

bool LevelState::isMetal(sf::Vector2f position) {
	if (position.x < 40 || position.x > 240 || position.y < 0 || position.y > 135) {
		return false;
	}
	else {
		return levelMask.getPixel(position.x - 40, position.y).g == 255;
	}
}

