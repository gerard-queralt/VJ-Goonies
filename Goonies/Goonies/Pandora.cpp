#include "Pandora.h"
#include "Game.h"

#define ANIM_TIME 3
#define VIS_TIME 115
#define INVIS_TIME 90
#define DMG_CD 40
#define HURT_CD 40
#define FLASH_TIME 3
#define FLASHING_TIME 40
#define RNG_TIME 300
#define VICTORY_LAP 300
#define SPEED 0.5

enum PandoraAnims{
	VISIBLE = 0, INVISBLE, NUM_ANIMS
};

enum PlayerStatus {
	GROUNDED = 0, JUMPING, FALLING, PUNCHING, CLIMBING
};

void Pandora::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/pandora.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(VISIBLE, ANIM_TIME);
	sprite->addKeyframe(VISIBLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(VISIBLE, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(INVISBLE, ANIM_TIME);
	sprite->addKeyframe(INVISBLE, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(INVISBLE, glm::vec2(0.5f, 0.5f));

	sprite->changeAnimation(VISIBLE);

	status = DEAD;

	hp = 10;

	invisTime = 0;
	dmgCD = DMG_CD;
	hurtCD = HURT_CD;
	flashing = false;
	flashTime = 0;
	flashingTime = 0;
	randomizeTime = 0;

	victoryLap = VICTORY_LAP;

	bubbles.resize(2);

	position = tileMapPos;
	startPosition = position;
	sprite->setPosition(position);

	for (int i = 0; i < bubbles.size(); ++i) {
		bubbles[i] = new PandoraBubble();
		bubbles[i]->init(glm::vec2(position.x - (16 * (-i * 2)), position.y), shaderProgram);
		bubbles[i]->setTileMap(map);
	}
}

void Pandora::update(int deltaTime)
{
	if (status != DEAD) {
		sprite->update(deltaTime);
		if (sprite->animation() == VISIBLE) {
			if (invisTime == VIS_TIME) {
				invisTime = 0;
				sprite->changeAnimation(INVISBLE);
			}
			else
				++invisTime;
		}
		else {
			if (invisTime == INVIS_TIME) {
				invisTime = 0;
				sprite->changeAnimation(VISIBLE);
			}
			else
				++invisTime;
		}
		if (horizontal == NEGATIVE) {
			position.x -= SPEED;
			if (position.x <= 0) {
				position.x = 0;
				horizontal = POSITIVE;
			}
		}
		else if(horizontal == POSITIVE){
			position.x += SPEED;
			if ((position.x + 32 /*size*/) >= (32 * map->getTileSize())) {
				position.x = (32 * map->getTileSize()) - 32 /*size*/;
				horizontal = NEGATIVE;
			}
		}
		if (vertical == NEGATIVE) {
			position.y -= SPEED;
			if (position.y <= 2 * map->getTileSize()) {
				position.y = 2 * map->getTileSize();
				vertical = POSITIVE;
			}
		}
		else if(vertical == POSITIVE){
			position.y += SPEED;
			if ((position.y + 32 /*size*/) >= (22 * map->getTileSize())) {
				position.y = (22 * map->getTileSize()) - 32 /*size*/;
				vertical = NEGATIVE;
			}
		}
		if (flashing) {
			if (flashTime == FLASH_TIME) {
				if (sprite->animation() == VISIBLE) {
					if (status == DYING)
						status = ALIVE;
					else if (status == ALIVE)
						status = DYING;
				}
				flashTime = 0;
			}
			else
				++flashTime;
			if (flashingTime == FLASHING_TIME) {
				flashing = false;
				flashTime = 0;
				flashingTime = 0;
				status = ALIVE;
			}
			else
				++flashingTime;
		}
		for (int i = 0; i < bubbles.size(); ++i) {
			bubbles[i]->update(deltaTime);
		}
		if (randomizeTime == RNG_TIME) {
			randomize();
			randomizeTime = 0;
		}
		else
			++randomizeTime;
		sprite->setPosition(position);
	}
	else {
		if (victoryLap == 0) {
			Game::instance().endGame();
		}
		else
			--victoryLap;
	}
}

void Pandora::render()
{
	if (status != DEAD && status != DYING) {
		sprite->render();
	}
	if (status != DEAD) {
		for (int i = 0; i < bubbles.size(); ++i) {
			bubbles[i]->render();
		}
	}
}

void Pandora::setIdle()
{
	status = DEAD;
	sprite->changeAnimation(VISIBLE);
	invisTime = 0;
	dmgCD = DMG_CD;
	hurtCD = HURT_CD;
	flashing = false;
	flashTime = 0;
	flashingTime = 0;
	victoryLap = VICTORY_LAP;
	position = startPosition;
	sprite->setPosition(startPosition);
}

void Pandora::setActive()
{
	status = ALIVE;
	sprite->changeAnimation(VISIBLE);
	randomize();
	invisTime = 0;
	dmgCD = DMG_CD;
	hurtCD = HURT_CD;
	flashing = false;
	flashTime = 0;
	flashingTime = 0;
	victoryLap = VICTORY_LAP;
	position = startPosition;
	sprite->setPosition(startPosition);
}

void Pandora::interact()
{
	if (status != DEAD) {
		if (dmgCD < DMG_CD)
			++dmgCD;
		if (hurtCD < HURT_CD)
			++hurtCD;
		if (inContactWithPlayer(position, glm::ivec2(32, 32))) {
			if (hurtCD == HURT_CD && player->getStatus() == PUNCHING && hitByPunch(position, glm::ivec2(32, 32))) {
				--hp;
				hurtCD = 0;
				flashing = true;
				if (hp == 0) {
					status = DEAD;
				}
			}
			else if (dmgCD == DMG_CD) {
				player->hurt(8);
				dmgCD = 0;
			}
		}
		for (int i = 0; i < bubbles.size(); ++i) {
			bubbles[i]->interact();
		}
	}
}

void Pandora::setPlayer(Player * player)
{
	this->player = player;
	for (int i = 0; i < bubbles.size(); ++i) {
		bubbles[i]->setPlayer(player);
	}
}

void Pandora::randomize()
{
	//1/9 de quedar-se quieta respecte un eix i 4/9 de moure's en cadascuna de les direccions
	horizontal = rand() % 9;
	vertical = rand() % 9;
	while (horizontal == STAY && vertical == STAY) {
		horizontal = rand() % 9;
		vertical = rand() % 9;
	}
	if (horizontal > STAY)
		horizontal = horizontal % 2 + 1;
	if (vertical > STAY)
		vertical = vertical % 2 + 1;
	for (int i = 0; i < bubbles.size(); ++i) {
		bubbles[i]->randomize();
	}
}
