#include "menu_state.hpp"

#include "level_state.hpp"
#include <cstdlib>

MenuState::MenuState() {

}

MenuState::~MenuState() {
	for (Particle *particle : particles) {
		delete particle;
	}
}

void MenuState::init() {
	sf::Texture &bg = loadTexture("img/menu.png");
	bg.setRepeated(true);
	background.setTexture(bg);

	title.setTexture(loadTexture("img/title.png"));
	title.setTextureRect(sf::IntRect(0, 0, 180, 40));
	title.setPosition(120 - 90, 0);

	text = BitmapText(loadTexture("img/font.png"), "Missions\nQuit\n");
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

	backgroundOffset += elapsed.asSeconds() * (1 - beatCounter) * 25;
	background.setTextureRect(sf::IntRect(0, backgroundOffset, 240, 135));

	createParticle(sf::Vector2f(54 + std::rand() % 131, 10 + std::rand() % 20), sf::Vector2f(std::rand() % 5 - 2, 8 + std::rand() % 4), 1.6 + std::rand() % 10 * .1);

	for (int i = particles.size() - 1; i >= 0; i--) {
		particles[i]->life -= elapsed.asSeconds();
		if (particles[i]->life <= 0) {
			delete particles[i];
			particles.erase(particles.begin() + i);
		}
		else {
			particles[i]->sprite.move(particles[i]->velocity * elapsed.asSeconds());
		}
	}
}

void MenuState::render(sf::RenderWindow &window) {
	window.draw(background);
	window.draw(title);
	window.draw(text);

	for (Particle *particle : particles) {
		window.draw(particle->sprite);
	}
}

void MenuState::createParticle(sf::Vector2f position, sf::Vector2f velocity, float lifespan) {
	Particle *temp = new Particle();
	temp->sprite.setTexture(loadTexture("img/particle.png"));
	temp->sprite.setPosition(position);
	temp->velocity = velocity;
	temp->life = lifespan;
	particles.push_back(temp);
}
