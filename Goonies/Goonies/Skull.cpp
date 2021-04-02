#include <iostream>
#include "Skull.h"

#define SPEED 1
#define DMG_CD 40

enum SkullAnims
{
	MOVE_LEFT = 0, MOVE_RIGHT, SPAWNING, DYING, NUM_ANIM
};

enum PlayerStatus {
	GROUNDED = 0, JUMPING, FALLING, PUNCHING, CLIMBING
};

void Skull::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/skull.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIM);

	sprite->setAnimationSpeed(MOVE_RIGHT, 7);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.5f));

	sprite->setAnimationSpeed(MOVE_LEFT, 7);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(SPAWNING, 7);
	sprite->addKeyframe(SPAWNING, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(SPAWNING, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(DYING, 7);
	sprite->addKeyframe(DYING, glm::vec2(0.75f, 0.f));

	sprite->changeAnimation(SPAWNING);
	movingLeft = true;

	status = DEAD;
	spawnTime = 60;
	deathTime = 10;
	dmgCD = DMG_CD;

	position = tileMapPos;
	startPosition = position;
	sprite->setPosition(position);
}

void Skull::update(int deltaTime)
{
	if (status == ALIVE) {
		sprite->update(deltaTime);
		if (movingLeft) {
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			position.x -= SPEED;
			if (inContactWithPlayer(position, glm::ivec2(16, 8))) { //potser cal una comprovacio especial per a que no mati d'esquena
				if (player->getStatus() == PUNCHING && status == ALIVE) {
					status = DYING;
					sprite->changeAnimation(DYING);
				}
				else if (dmgCD == DMG_CD) {
					player->hurt(5); //nombre random
					dmgCD = 0;
				}
					
			}
			else if (map->collisionMoveLeft(position, glm::ivec2(16, 8)) ||
				position.x < 0) {
				position.x += SPEED;
				movingLeft = false;
			}
		}
		else {
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			position.x += SPEED;
			if(inContactWithPlayer(position, glm::ivec2(16, 8))) { //potser cal una comprovacio especial per a que no mati d'esquena
				if (player->getStatus() == PUNCHING && status == ALIVE) {
					status = DYING;
					sprite->changeAnimation(DYING);
				}
				else if (dmgCD == DMG_CD) {
					player->hurt(5); //nombre random
					dmgCD = 0;
				}
			}
			else if (map->collisionMoveRight(position, glm::ivec2(16, 8)) ||
				position.x > (map->getMapSize().x - 2) * map->getTileSize()) {
				position.x -= SPEED;
				movingLeft = true;
			}
		}
		sprite->setPosition(position);
	}
	else if (status == SPAWNING) {
		sprite->update(deltaTime);
		--spawnTime;
		if (spawnTime == 0)
			status = ALIVE;
	}
	else if (status == DYING) {
		sprite->update(deltaTime);
		--deathTime;
		if (deathTime == 0)
			status = DEAD;
	}
	if (dmgCD < DMG_CD)
		++dmgCD;
}

void Skull::setIdle() {
	status = DEAD;
	spawnTime = 60;
	deathTime = 10;
}

void Skull::setActive()
{
	status = SPAWNING;
	position = startPosition;
	sprite->setPosition(position);
	sprite->changeAnimation(SPAWNING);
	movingLeft = true;
	spawnTime = 60;
	deathTime = 10;
}

