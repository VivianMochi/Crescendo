#pragma once

#include <SFML/Graphics.hpp>

class State;

class BeatBlock : public sf::Drawable {
public:
	BeatBlock(sf::IntRect box);
	~BeatBlock();

	void setState(State *state);
	void init();
	void update(sf::Time elapsed);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	State *state;
};

