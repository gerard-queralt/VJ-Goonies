#include <iostream>
#include "Skull.h"

#define SPEED 1
#define JUMP_SPEED 1.5
#define SPAWN_TIME 60
#define DMG_CD 40
#define MAX_TILE_JUMP 3

enum SkullAnims {
	MOVE_LEFT = 0, MOVE_RIGHT, SPAWNING, DYING, NUM_ANIM
};

enum PlayerStatus {
	GROUNDED = 0, JUMPING, FALLING, PUNCHING, CLIMBING
};

enum Behaviours {
	STRAIGHT = 0, JUMPY
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
	falling = false;
	spawnTime = SPAWN_TIME;
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
		if (behaviour == JUMPY) {
			if (!falling) {
				position.y -= JUMP_SPEED;
				if (position.y <= (startPosition.y - MAX_TILE_JUMP * map->getTileSize()) || map->collisionMoveUp(position, glm::vec2(16, 8)))
					falling = true;
			}
			else {
				position.y += JUMP_SPEED;
				if (position.y >= startPosition.y) {
					position.y = startPosition.y;
					falling = false;
				}
			}
		}
		if (movingLeft) {
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			position.x -= SPEED;
			if (inContactWithPlayer(position, glm::ivec2(16, 8))) {
				if ((player->getStatus() == PUNCHING && hitByPunch(position, glm::ivec2(16, 8))) || player->getPowerUps()[4] /*green spellbook*/) {
					status = DYING;
					sprite->changeAnimation(DYING);
					player->gainXP(5);
				}
				else if (dmgCD == DMG_CD) {
					player->hurt(5);
					dmgCD = 0;
				}
					
			}
			if (map->collisionMoveLeft(position, glm::ivec2(16, 8)) ||
				position.x < 0 ||
				!map->collisionMoveDownEntities(glm::vec2(position.x - 8, startPosition.y), glm::ivec2(16,12))) {
				position.x += SPEED;
				movingLeft = false;
			}
		}
		else {
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			position.x += SPEED;
			if(inContactWithPlayer(position, glm::ivec2(16, 8))) {
				if ((player->getStatus() == PUNCHING && hitByPunch(position, glm::ivec2(16, 8))) || player->getPowerUps()[4] /*green spellbook*/) {
					status = DYING;
					sprite->changeAnimation(DYING);
					player->gainXP(5);
				}
				else if (dmgCD == DMG_CD) {
					player->hurt(5);
					dmgCD = 0;
				}
			}
			if (map->collisionMoveRight(position, glm::ivec2(16, 8)) ||
				position.x > (map->getMapSize().x - 2) * map->getTileSize() ||
				!map->collisionMoveDownEntities(glm::vec2(position.x + 8, startPosition.y), glm::ivec2(16, 12))) {
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
	spawnTime = SPAWN_TIME;
	deathTime = 10;
}

void Skull::setActive()
{
	status = SPAWNING;
	falling = false;
	position = startPosition;
	sprite->setPosition(position);
	sprite->changeAnimation(SPAWNING);
	movingLeft = true;
	spawnTime = SPAWN_TIME;
	deathTime = 10;
}

void Skull::setBehaviour(int bhv)
{
	behaviour = bhv;
}

