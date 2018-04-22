#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class State;

class Lever : public sf::Drawable {
public:
	Lever(State *state, sf::Vector2f position, bool facingRight = false);
	~Lever();

	void flip();
	bool isFlipped();

	bool isOverlapping(sf::Vector2f position);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	State *state;

	sf::Vector2f position;
	bool facingRight;
	bool flipped = false;

	sf::Sprite sprite;

	sf::Sound flipSound;
};

