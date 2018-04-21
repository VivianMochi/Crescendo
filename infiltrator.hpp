#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class State;

class Infiltrator : public sf::Drawable {
	static const int GRAVITY = 400;
	static const int SPEED = 80;
	static const int ACCELERATION = 300;
	static const int JUMP = 180;
	static const int DASH = 25;

public:
	void setState(State *state);
	void init();
	void update(sf::Time elapsed);

	float getVolume() const;

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	void setVolume(float volume);

	State *state = nullptr;
	sf::Sprite sprite;

	sf::Vector2f position;
	sf::Vector2f velocity;
	bool onGround = false;
	bool facingRight = true;
	int frame = 0;
	float frameCounter = 0;
	bool dash = false;
	float dashCooldown = 0;

	float volume = 0;

	sf::Sound softStepSound;
	sf::Sound loudStepSound;
	sf::Sound jumpSound;
	sf::Sound dashSound;
};

