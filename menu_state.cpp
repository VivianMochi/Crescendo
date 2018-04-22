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

	text = BitmapText(loadTexture("img/font.png"), "Missions\nCredits\nQuit\n");
	text.setPosition(50, 50);

	music.openFromFile("mus/menu.ogg");
	music.setLoop(true);
	music.play();
}

void MenuState::gotEvent(sf::Event event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Return) {
			game->changeState(new LevelState(1));
		}
	}
}

void MenuState::update(sf::Time elapsed) {
	beatCounter += elapsed.asSeconds();
	if (beatCounter >= 120.f / 130.f) {
		beatCounter -= 120.f / 130.f;
	}
}

void MenuState::render(sf::RenderWindow &window) {
	if (beatCounter > .1)
		window.draw(title);
	window.draw(text);
}