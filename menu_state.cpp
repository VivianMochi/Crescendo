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

	text = BitmapText(loadTexture("img/font.png"), "Press Enter to begin");
	text.setPosition(51, 80);

	creditText = BitmapText(loadTexture("img/font.png"), "A Game by Nate Largo");
	creditText.setPosition(50, 125);

	loading = BitmapText(loadTexture("img/font.png"), "Loading...");
	loading.setColor(sf::Color::White);
	loading.setPosition(183, 125);

	elevator.setTexture(loadTexture("img/elevator.png"));
	elevator.setTextureRect(sf::IntRect(0, 0, 40, 135));
	elevator.setPosition(-40, 0);

	startSound.setBuffer(loadSoundBuffer("snd/start.wav"));

	music.openFromFile("mus/menu.ogg");
	music.setLoop(true);
	music.play();
}

void MenuState::gotEvent(sf::Event event) {
	if (event.type == sf::Event::KeyPressed) {
		if (menuState == 0 && event.key.code == sf::Keyboard::Return) {
			menuState = 1;
			beatCounter = 2;
			startSound.play();
		}
	}
}

void MenuState::update(sf::Time elapsed) {
	if (menuState == 0) {
		beatCounter += elapsed.asSeconds();
		if (beatCounter >= 120.f / 130.f) {
			beatCounter -= 120.f / 130.f;
		}

		backgroundOffset += elapsed.asSeconds() * (1 - beatCounter) * 25;
		background.setTextureRect(sf::IntRect(0, backgroundOffset, 240, 135));

		createParticle(sf::Vector2f(54 + std::rand() % 131, 10 + std::rand() % 20), sf::Vector2f(std::rand() % 5 - 2, 8 + std::rand() % 4), 1.6 + std::rand() % 10 * .1);
	}
	else {
		beatCounter -= elapsed.asSeconds();
		if (beatCounter <= -.1) {
			game->changeState(new LevelState(1));
		}

		if (beatCounter > 0) {
			background.setScale(beatCounter / 2, 1);
			background.setPosition(120 * (1 - beatCounter / 2), 0);

			elevator.setPosition(-40 * std::pow(0.09, 2 - beatCounter), 0);

			music.setVolume(100 * beatCounter / 2);
		}
		else {
			elevator.setPosition(0, 0);
			music.stop();
		}
	}

	// Update particles
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
	if (menuState == 0 || beatCounter > 0) {
		window.draw(background);
	}
	window.draw(title);
	if (menuState == 0 && beatCounter >= 60.f / 130.f) {
		window.draw(text);
	}
	window.draw(creditText);

	for (Particle *particle : particles) {
		window.draw(particle->sprite);
	}
	
	if (menuState == 1) {
		window.draw(elevator);
		if (beatCounter <= 0) {
			window.draw(loading);
		}
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
