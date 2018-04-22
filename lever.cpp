#include "lever.hpp"

#include "state.hpp"

Lever::Lever(State *state, sf::Vector2f position, bool facingRight) : state(state), position(position), facingRight(facingRight) {
	sprite.setTexture(state->loadTexture("img/lever.png"));
	sprite.setTextureRect(sf::IntRect(0, 0, 10, 10));
	if (facingRight) {
		sprite.setScale(-1, 1);
	}
	sprite.setPosition(position + (facingRight ? sf::Vector2f(5, -5) : sf::Vector2f(-5, -5)));

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
	return std::abs(position.x - this->position.x) < 10 && std::abs(position.y - this->position.y) < 10;
}

void Lever::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(sprite, states);
}
