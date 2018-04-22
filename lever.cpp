#include "lever.hpp"

#include "state.hpp"

Lever::Lever(State *state, sf::Vector2f position, bool facingRight) : state(state), position(position), facingRight(facingRight) {
	sprite.setTexture(state->loadTexture("img/lever.png"));
	sprite.setTextureRect(sf::IntRect(0, 0, 10, 10));
	if (facingRight) {
		sprite.setScale(-1, 1);
	}
	sprite.setPosition(position + (facingRight ? sf::Vector2f(8, -13) : sf::Vector2f(-2, -13)));

	flipSound.setBuffer(state->loadSoundBuffer("snd/door.wav"));
}

Lever::~Lever() {

}

void Lever::flip() {
	flipped = true;
	sprite.setTextureRect(sf::IntRect(10, 0, 10, 10));
	flipSound.play();
}

bool Lever::isFlipped() {
	return flipped;
}

bool Lever::isOverlapping(sf::Vector2f position) {
	sf::Vector2f center = this->position + sf::Vector2f(3, -5);
	return std::abs(position.x - center.x) < 10 && std::abs(position.y - center.y) < 10;
}

void Lever::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(sprite, states);
}
