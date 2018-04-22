#include "orb.hpp"

#include "state.hpp"

Orb::Orb() {

}

Orb::Orb(State *state, sf::Vector2f position) : state(state), position(position) {
	sprite.setTexture(state->loadTexture("img/orb.png"));
	sprite.setTextureRect(sf::IntRect(0, 0, 10, 14));
	sprite.setPosition(position + sf::Vector2f(0, -14));
}

Orb::~Orb() {

}

void Orb::setBeat(int beat) {
	if (!collected) {
		if (beat % 4 == 3) {
			sprite.setTextureRect(sf::IntRect(10, 0, 10, 14));
		}
		else {
			sprite.setTextureRect(sf::IntRect(beat % 4 * 10, 0, 10, 14));
		}
	}
}

void Orb::collect() {
	collected = true;
	sprite.setTextureRect(sf::IntRect(30, 0, 10, 14));
}

bool Orb::isCollected() {
	return collected;
}

bool Orb::isOverlapping(sf::Vector2f position) {
	sf::Vector2f center = this->position + sf::Vector2f(5, -7);
	return std::abs(position.x - center.x) < 10 && std::abs(position.y - center.y) < 10;
}

void Orb::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(sprite, states);
}
