#include "menu_state.hpp"

#include "level_state.hpp"

MenuState::MenuState() {

}

MenuState::~MenuState() {

}

void MenuState::init() {
	title.setTexture(loadTexture("img/title.png"));
	title.setTextureRect(sf::IntRect(0, 0, 180, 40));
	title.setPosition(120 - 90, 0);

	text = BitmapText(loadTexture("img/font.png"), "Begin\nHelp\nQuit\n");
	text.setPosition(50, 50);
}

void MenuState::gotEvent(sf::Event event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Return) {
			game->changeState(new LevelState(1));
		}
	}
}

void MenuState::update(sf::Time elapsed) {

}

void MenuState::render(sf::RenderWindow &window) {
	window.draw(title);
	window.draw(text);
}