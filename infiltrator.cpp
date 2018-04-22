#include "infiltrator.hpp"

#include "state.hpp"
#include "level_state.hpp"

void Infiltrator::setState(State *state) {
	this->state = state;
}

void Infiltrator::init() {
	position.x = 19;
	position.y = 112.999;

	sprite.setTexture(state->loadTexture("img/infiltrator.png"));
	sprite.setTextureRect(sf::IntRect(0, 0, 14, 14));

	softStepSound.setBuffer(state->loadSoundBuffer("snd/soft_step.wav"));
	loudStepSound.setBuffer(state->loadSoundBuffer("snd/loud_step.wav"));
	jumpSound.setBuffer(state->loadSoundBuffer("snd/jump.wav"));
	dashSound.setBuffer(state->loadSoundBuffer("snd/dash.wav"));
}

void Infiltrator::update(sf::Time elapsed) {
	float seconds = elapsed.asSeconds();

	// Tick variables
	if (volumeFreeze > 0) {
		volumeFreeze -= seconds;
	}
	else {
		volume *= std::pow(.1, seconds);
	}

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
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) && onGround) {
		velocity.y -= JUMP;
		jumpSound.play();
		setVolume(2);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && dashCooldown <= 0) {
		dash = true;
		dashSound.play();
		setVolume(3);
	}

	// Do movement
	sf::Vector2f tempPosition = position;
	tempPosition.x += velocity.x * seconds;
	if (dash) {
		dashCooldown += .4;
		tempPosition.x += facingRight ? DASH : -DASH;
		velocity.x += facingRight ? SPEED : -SPEED;
	}
	if (((LevelState*)state)->checkCollision(tempPosition, 4, 12)) {
		do {
			if (velocity.x > 0) {
				tempPosition.x -= 1;
				if (tempPosition.x < position.x) {
					tempPosition.x = position.x;
					break;
				}
			}
			else {
				tempPosition.x += 1;
				if (tempPosition.x > position.x) {
					tempPosition.x = position.x;
					break;
				}
			}
		} while (((LevelState*)state)->checkCollision(tempPosition, 4, 12));
		position.x = tempPosition.x;
		velocity.x = 0;
	}
	else {
		position.x = tempPosition.x;
	}
	tempPosition.y += velocity.y * seconds;
	if (((LevelState*)state)->checkCollision(tempPosition, 4, 12)) {
		do {
			if (velocity.y > 0) {
				tempPosition.y -= 1;
				if (tempPosition.y < position.y) {
					tempPosition.y = position.y;
					break;
				}
			}
			else {
				tempPosition.y += 1;
				if (tempPosition.y > position.y) {
					tempPosition.y = position.y;
					break;
				}
			}
		} while (((LevelState*)state)->checkCollision(tempPosition, 4, 12));
		position.y = tempPosition.y;

		if (velocity.y > 0 && !onGround) {
			onGround = true;
			position.y = std::ceil(position.y) - .001;
			if (((LevelState*)state)->isMetal(position + sf::Vector2f(2, 13))) {
				loudStepSound.play();
				setVolume(2);
			}
			else {
				softStepSound.play();
				setVolume(1);
			}
		}
		velocity.y = 0;
	}
	else {
		position.y = tempPosition.y;
		onGround = false;
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
					if (((LevelState*)state)->isMetal(position + sf::Vector2f(2, 13))) {
						loudStepSound.play();
						setVolume(2);
					}
					else {
						softStepSound.play();
						setVolume(1);
					}
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

	sprite.setTextureRect(sf::IntRect(14 * frame, carryingOrb ? 14 : 0, 14, 14));
	sprite.setPosition(position + sf::Vector2f(facingRight ? -5 : 9, -2));
}

sf::Vector2f Infiltrator::getCenterPosition() {
	return position + sf::Vector2f(2, 6);
}

float Infiltrator::getVolume() const {
	return volume;
}

void Infiltrator::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(sprite, states);
}

void Infiltrator::setVolume(float volume) {
	if (volume > this->volume) {
		this->volume = volume;
		volumeFreeze = .15;
	}
}
