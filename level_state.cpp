#include "level_state.hpp"

#include <iomanip>
#include <sstream>
#include <fstream>

LevelState::LevelState(int level, bool skipIntro) : level(level) {
	if (skipIntro) {
		gameState = 1;
	}
	else {
		gameState = 0;
	}
}

LevelState::~LevelState() {
	for (Lever *lever : levers) {
		delete lever;
	}

	for (BeatBlock *block : blocks) {
		delete block;
	}

	for (sf::Music *mus : music) {
		delete mus;
	}
}

void LevelState::init() {
	elevator.setTexture(loadTexture("img/elevator.png"));
	elevator.setTextureRect(sf::IntRect(0, 0, 40, 135));

	floorDisplay.setTexture(loadTexture("img/font.png"));
	floorDisplay.setPosition(13, 98);
	floorDisplay.setColor(sf::Color(127, 0, 0));
	std::stringstream ss;
	ss << "lv" << level;
	floorDisplay.setText(ss.str());
	ss.str("");

	ss << "level/level" << level << ".png";
	levelSprite.setTexture(loadTexture(ss.str()));
	ss.str("");

	ss << "level/level" << level << "m.png";
	levelMask.loadFromFile(ss.str());
	ss.str("");

	player.setState(this);
	player.init();

	hud.setTexture(loadTexture("img/hud.png"));
	hud.setTextureRect(sf::IntRect(0, 0, 36, 91));
	ambientBar.setTexture(loadTexture("img/hud.png"));
	personalBar.setTexture(loadTexture("img/hud.png"));
	alertOverlay.setTexture(loadTexture("img/alert.png"));

	pauseOverlay.setTexture(loadTexture("img/font.png"));
	pauseOverlay.setPosition(78, 8);
	pauseOverlay.setColor(sf::Color(127, 0, 0));
	pauseOverlay.setText("PAUSED - ESC to Resume");

	elevatorSound.setBuffer(loadSoundBuffer("snd/elevator.wav"));
	doorSound.setBuffer(loadSoundBuffer("snd/door.wav"));
	alertSound.setBuffer(loadSoundBuffer("snd/alert.wav"));
	completeSound.setBuffer(loadSoundBuffer("snd/complete.wav"));

	for (int i = 0; i < (level == 5 ? 1 : 3); i++) {
		music.push_back(new sf::Music());
		ss << "mus/level" << level << "-" << i + 1 << ".ogg";
		music[i]->openFromFile(ss.str());
		ss.str("");
		music[i]->setVolume(0);
		music[i]->setLoop(true);
	}
	music[0]->setVolume(100);

	staticMusic.openFromFile("mus/static.ogg");
	staticMusic.setLoop(true);
	staticMusic.setVolume(40);

	if (gameState == 1) {
		for (sf::Music *mus : music) {
			mus->play();
		}
		elevator.setTextureRect(sf::IntRect(40, 0, 40, 135));
	}

	setupLevel();
}

void LevelState::gotEvent(sf::Event event) {
	if (event.type == sf::Event::LostFocus) {
		if (gameState == 1) {
			gameState = 4;
			gameTimer = 0;
			for (sf::Music *mus : music) {
				mus->pause();
			}
			staticMusic.pause();
		}
	}
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::X && gameState == 1) {
			if (!orb.isCollected() && orb.isOverlapping(player.getCenterPosition())) {
				orb.collect();
				player.carryingOrb = true;
				for (sf::Music *mus : music) {
					mus->stop();
				}
				staticMusic.play();
			}
			for (Lever *lever : levers) {
				if (!lever->isFlipped() && lever->isOverlapping(player.getCenterPosition())) {
					lever->flip();
					setSection(section + 1);
				}
			}
		}
		else if (event.key.code == sf::Keyboard::Escape) {
			if (gameState == 1) {
				gameState = 4;
				gameTimer = 0;
				for (sf::Music *mus : music) {
					mus->pause();
				}
				staticMusic.pause();
			}
			else if (gameState == 4) {
				gameState = 1;
				if (orb.isCollected()) {
					staticMusic.play();
				}
				else {
					for (sf::Music *mus : music) {
						mus->play();
					}
				}
			}
		}

		// Debug Keys
		/*if (event.key.code == sf::Keyboard::Num1) {
			setSection(1);
		}
		else if (event.key.code == sf::Keyboard::Num2) {
			setSection(2);
		}
		else if (event.key.code == sf::Keyboard::Num3) {
			setSection(3);
		}
		else if (event.key.code == sf::Keyboard::Num9) {
			if (level > 1) {
				game->changeState(new LevelState(level - 1));
			}
		}
		else if (event.key.code == sf::Keyboard::Num0) {
			game->changeState(new LevelState(level + 1));
		}*/
	}
}

void LevelState::update(sf::Time elapsed) {
	if (gameState == 0) {
		gameTimer -= elapsed.asSeconds();
		if (gameTimer <= 1.5 && elevatorSound.getStatus() == sf::Sound::Stopped) {
			elevatorSound.play();
		}
		if (gameTimer <= 0) {
			gameState = 1;
			for (sf::Music *mus : music) {
				mus->play();
			}
			elevator.setTextureRect(sf::IntRect(40, 0, 40, 135));
			doorSound.play();
		}
	}
	if (gameState == 1) {
		// Tick variables
		beatCounter += elapsed.asSeconds();
		if (beatCounter >= 60.f / bpm) {
			beatCounter -= 60.f / bpm;
			beat++;
			orb.setBeat(beat);
		}
		volume *= std::pow(.5, elapsed.asSeconds());

		// Calculate music volume
		calculateVolume();

		// Update player
		player.update(elapsed);

		if (player.carryingOrb && player.getCenterPosition().x <= 23) {
			gameState = 3;
			gameTimer = 2.5;
			staticMusic.stop();
			elevator.setTextureRect(sf::IntRect(0, 0, 40, 135));
			completeSound.play();
		}

		// Calculate alert
		if (player.getVolume() <= volume && alert > 0) {
			alert -= elapsed.asSeconds();
			if (alert < 0) {
				alert = 0;
			}
		}
		else if (player.getVolume() > volume) {
			alert += elapsed.asSeconds();
		}
		if (alert >= .15) {
			gameState = 2;
			gameTimer = .6;
			for (sf::Music *mus : music) {
				mus->stop();
			}
			staticMusic.stop();
			alertSound.play();
		}

		// Update hud
		if (volume > 0) {
			float tempVolume = volume;
			if (tempVolume > 4) {
				tempVolume = 4;
			}
			int height = 69 * tempVolume / 4;
			ambientBar.setPosition(8, 18 + (69 - height));
			ambientBar.setTextureRect(sf::IntRect(36, 69 - height, 11, height));
		}
		if (player.getVolume() > 0) {
			float tempVolume = player.getVolume();
			if (tempVolume > 4) {
				tempVolume = 4;
			}
			int height = 69 * tempVolume / 4;
			personalBar.setPosition(21, 18 + (69 - height));
			personalBar.setTextureRect(sf::IntRect(47, 69 - height, 11, height));
		}
	}
	if (gameState == 2) {
		gameTimer -= elapsed.asSeconds();
		if (gameTimer <= 0) {
			game->changeState(new LevelState(level, 1));
		}
	}
	if (gameState == 3) {
		gameTimer -= elapsed.asSeconds();
		if (gameTimer <= 0) {
			if (level == 5) {
				game->exit();
			}
			else {
				game->changeState(new LevelState(level + 1));
			}
		}
	}
	if (gameState == 4) {
		gameTimer += elapsed.asSeconds();
		if (gameTimer >= 1.6) {
			gameTimer -= 1.6;
		}
	}
}

void LevelState::render(sf::RenderWindow &window) {
	window.draw(elevator);
	if (gameState == 0) {
		if (gameTimer < 1.5) {
			window.draw(floorDisplay);
		}
	}
	else if (gameState <= 2 || gameState == 4) {
		if (beat % 4 <= 2) {
			window.draw(floorDisplay);
		}
		window.draw(levelSprite);

		for (Lever *lever : levers) {
			window.draw(*lever);
		}

		for (BeatBlock *block : blocks) {
			window.draw(*block);
		}

		window.draw(orb);

		window.draw(player);

		window.draw(hud);
		if (volume > 0) {
			window.draw(ambientBar);
		}
		if (player.getVolume()) {
			window.draw(personalBar);
		}

		if (gameState == 2) {
			window.draw(alertOverlay);
		}
		else if (gameState == 4 && gameTimer <= .8) {
			window.draw(pauseOverlay);
		}
	}
	else if (gameState == 3) {
		if (gameTimer > 0.5) {
			window.draw(floorDisplay);
		}
	}
}

bool LevelState::checkCollision(sf::Vector2f position, int width, int height) {
	if (position.y + height > 125) {
		return true;
	}
	else if (position.y < 10) {
		return true;
	}
	else if (position.x + width > 230) {
		return true;
	}
	else if (position.x < 10) {
		return true;
	}
	else if (position.x < 40 && position.y < 95) {
		return true;
	}
	else if (position.x > 40 && position.x + width < 240 && position.y > 0 && position.y + height < 135) {
		if (levelMask.getPixel(position.x, position.y).b == 255) {
			return true;
		}
		else if (levelMask.getPixel(position.x + width, position.y).b == 255) {
			return true;
		}
		else if (levelMask.getPixel(position.x, position.y + height / 2).b == 255) {
			return true;
		}
		else if (levelMask.getPixel(position.x + width, position.y + height / 2).b == 255) {
			return true;
		}
		else if (levelMask.getPixel(position.x, position.y + height).b == 255) {
			return true;
		}
		else if (levelMask.getPixel(position.x + width, position.y + height).b == 255) {
			return true;
		}
	}
	return false;
}

bool LevelState::isMetal(sf::Vector2f position) {
	if (position.x < 40 || position.x > 240 || position.y < 0 || position.y > 135) {
		return false;
	}
	else {
		return levelMask.getPixel(position.x, position.y).g == 255;
	}
}

void LevelState::setupLevel() {
	if (level == 1) {
		levers.push_back(new Lever(this, sf::Vector2f(87, 125)));
		levers.push_back(new Lever(this, sf::Vector2f(160, 47), true));
		orb = Orb(this, sf::Vector2f(44, 47));
	}
	else if (level == 2) {
		levers.push_back(new Lever(this, sf::Vector2f(165, 125)));
		levers.push_back(new Lever(this, sf::Vector2f(46, 77), true));
		orb = Orb(this, sf::Vector2f(218, 125));
	}
	else if (level == 3) {
		levers.push_back(new Lever(this, sf::Vector2f(42, 33), true));
		levers.push_back(new Lever(this, sf::Vector2f(100, 125), true));
		orb = Orb(this, sf::Vector2f(218, 93));
	}
	else if (level == 4) {
		levers.push_back(new Lever(this, sf::Vector2f(188, 94)));
		levers.push_back(new Lever(this, sf::Vector2f(188, 38)));
		orb = Orb(this, sf::Vector2f(218, 68));
	}
	else if (level == 5) {
		orb = Orb(this, sf::Vector2f(218, 125));
	}
}

void LevelState::calculateVolume() {
	if (level == 1) {
		if (beatCounter < .15) {
			int localBeat = beat % 16;
			if (section >= 1) {
				if ((localBeat % 2 == 0 && localBeat != 14) || localBeat == 9) {
					setVolume(1.8);
				}
			}
			if (section >= 2) {
				if ((localBeat % 2 == 0 && localBeat != 14) || localBeat == 11 || beat % 32 == 25) {
					setVolume(2.5);
				}
			}
			if (section == 3) {
				if (localBeat % 2 == 0 && localBeat < 8) {
					setVolume(3.4);
				}
			}
		}
	}
	else if (level == 2) {
		if (beatCounter < .15) {
			int localBeat = beat % 16;
			if (section >= 1) {
				if (localBeat % 4 == 0 && localBeat != 12) {
					setVolume(1.8);
				}
			}
			if (section >= 2) {
				if (localBeat % 2 == 0) {
					setVolume(2.4);
				}
			}
			if (section == 3) {
				if (localBeat == 0) {
					setVolume(3.7);
				}
			}
		}
	}
	else if (level == 3) {
		if (beatCounter < .15) {
			int localBeat = beat % 16;
			if (section >= 1) {
				setVolume(1.4);
				if (localBeat % 4 == 0) {
					setVolume(2.3);
				}
			}
			if (section >= 2) {
				if (beat % 32 < 20) {
					if (localBeat % 4 == 0 || beat % 32 == 14 || beat % 32 == 18) {
						setVolume(2.6);
					}
				}
			}
			if (section == 3) {
				if (localBeat % 4 == 0) {
					setVolume(3.4);
				}
			}
		}
		else if (beatCounter > 30.f / bpm && beatCounter < 30.f / bpm + .15) {
			// Set offbeats
			int localBeat = beat % 32;
			if (section >= 2 && localBeat < 20) {
				if (localBeat == 2 || localBeat == 7 || localBeat == 9 || localBeat == 10 || localBeat == 13 || localBeat == 15 || localBeat == 17 || localBeat == 19) {
					setVolume(2.6);
				}
			}
		}
		else if (beatCounter > 15.f / bpm && beatCounter < 15.f / bpm + .15) {
			// Set second fourths
			int localBeat = beat % 32;
			if (section >= 2 && localBeat < 20) {
				if (localBeat == 3 || localBeat == 11) {
					setVolume(2.6);
				}
			}
		}
		else if (beatCounter > 45.f / bpm && beatCounter < 45.f / bpm + .15) {
			// Set fourth fourths
			int localBeat = beat % 32;
			if (section >= 2 && localBeat < 20) {
				if (localBeat == 7 || localBeat == 12 || localBeat == 14 || localBeat == 17 || localBeat == 18) {
					setVolume(2.6);
				}
			}
		}
	}
	else if (level == 4) {
		if (beatCounter < .15) {
			int localBeat = beat % 16;
			if (section >= 1) {
				if (localBeat % 2 == 0) {
					setVolume(2.3);
				}
			}
			if (section == 3) {
				if (localBeat < 8) {
					setVolume(3.2);
				}
			}
		}
		if (section >= 2) {
			int localBeat = beat % 16;
			if (localBeat < 6) {
				setVolume(2.3 + (5 - localBeat) * 0.1);
			}
		}
	}
	else if (level == 5) {
		if (beatCounter < .15) {
			int localBeat = beat % 32;
			setVolume(2.5);
			if (localBeat % 4 == 0 || localBeat == 14 || localBeat == 15 || localBeat == 18 || localBeat == 19 || localBeat == 22 || localBeat == 26 || localBeat == 27) {
				setVolume(3.8);
			}
		}
		else if (beatCounter > 30.f / bpm && beatCounter < 30.f / bpm + .15) {
			setVolume(2.5);
		}
	}
}

void LevelState::setVolume(float volume) {
	if (volume > this->volume) {
		this->volume = volume;
	}
}

void LevelState::setSection(int section) {
	music[this->section - 1]->setVolume(0);
	music[section - 1]->setVolume(100);
	this->section = section;
}

