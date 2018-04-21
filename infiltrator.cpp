#include "infiltrator.hpp"

#include "state.hpp"
#include "level_state.hpp"

void Infiltrator::setState(State *state) {
	this->state = state;
}

void Infiltrator::init() {
	sprite.setTexture(state->loadTexture("img/infiltrator.png"));
	sprite.setTextureRect(sf::IntRect(0, 0, 14, 14));

	softStepSound.setBuffer(state->loadSoundBuffer("snd/softStep.wav"));
	loudStepSound.setBuffer(state->loadSoundBuffer("snd/loudStep.wav"));
	jumpSound.setBuffer(state->loadSoundBuffer("snd/jump.wav"));
	dashSound.setBuffer(state->loadSoundBuffer("snd/dash.wav"));
}

void Infiltrator::update(sf::Time elapsed) {
	float seconds = elapsed.asSeconds();

	// Tick variables
	velocity.y += GRAVITY * seconds;

	if (dashCooldown > 0) {
		dashCooldown -= seconds;
	}

	// Player input
	dash = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		if (velocity.x < SPEED) {
			velocity.x += ACCELERATION * seconds;
		}
		else {
			velocity.x -= ACCELERATION * seconds;
		}
		facingRight = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		if (velocity.x > -SPEED) {
			velocity.x -= ACCELERATION * seconds;
		}
		else {
			velocity.x += ACCELERATION * seconds;
		}
		facingRight = false;
	}
	else {
		if (velocity.x > 0) {
			velocity.x -= ACCELERATION * seconds;
			if (velocity.x < 0) {
				velocity.x = 0;
			}
		}
		else if (velocity.x < 0) {
			velocity.x += ACCELERATION * seconds;
			if (velocity.x > 0) {
				velocity.x = 0;
			}
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && onGround) {
		velocity.y -= JUMP;
		jumpSound.play();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && dashCooldown <= 0) {
		dash = true;
		dashSound.play();
	}

	// Do movement
	onGround = false;
	sf::Vector2f tempPosition = position;
	tempPosition.x += velocity.x * seconds;
	if (dash) {
		dashCooldown += 1.5;
		tempPosition.x += facingRight ? DASH : -DASH;
		velocity.x += facingRight ? SPEED : -SPEED;
	}
	if (((LevelState*)state)->checkCollision(tempPosition, 4, 12)) {
		tempPosition = position;
		velocity.x = 0;
	}
	else {
		position = tempPosition;
	}
	tempPosition.y += velocity.y * seconds;
	if (((LevelState*)state)->checkCollision(tempPosition, 4, 12)) {
		tempPosition = position;
		if (velocity.y > 0) {
			onGround = true;
		}
		velocity.y = 0;
	}
	else {
		position = tempPosition;
	}

	// Update sprite
	if (facingRight) {
		sprite.setScale(1, 1);
	}
	else {
		sprite.setScale(-1, 1);
	}
	if (onGround) {
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			frame = 0;
		}
		else {
			if (frame < 1 || frame > 4) {
				frame = 1;
			}
			frameCounter += seconds;
			if (frameCounter > .05) {
				frameCounter = 0;
				frame++;
				if (frame == 2) {
					softStepSound.play();
				}
				else if (frame > 4) {
					frame = 1;
				}
			}
		}
	}
	else {
		frame = 5;
	}

	sprite.setTextureRect(sf::IntRect(14 * frame, 0, 14, 14));
	sprite.setPosition(position + sf::Vector2f(facingRight ? -5 : 9, -2));
}

void Infiltrator::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(sprite, states);
}
