#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class State;

class Orb : public sf::Drawable {
public:
	Orb();
	Orb(State *state, sf::Vector2f position);
	~Orb();

	void setBeat(int beat);
	void collect();
	bool isCollected();

	bool isOverlapping(sf::Vector2f position);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	State *state;

	sf::Vector2f position;
	bool collected = false;

	sf::Sprite sprite;
};

