#include "level_state.hpp"

LevelState::LevelState(int level) : level(level) {

}

LevelState::~LevelState() {

}

void LevelState::init() {
	player.setState(this);
	player.init();

	music.openFromFile("mus/tutorial.ogg");
	music.setLoop(true);
	music.play();
}

void LevelState::gotEvent(sf::Event event) {

}

void LevelState::update(sf::Time elapsed) {
	player.update(elapsed);
}

void LevelState::render(sf::RenderWindow &window) {
	window.draw(player);
}

bool LevelState::checkCollision(sf::Vector2f position, int width, int height) {
	if (position.y + height > 100) {
		return true;
	}
	return false;
}

void LevelState::emitSound(sf::Vector2f position, int volume) {
	
}
