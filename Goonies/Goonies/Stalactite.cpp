#include <iostream>
#include "Stalactite.h"

#define FALL_SPEED 1

enum StalAnims
{
	ALIVE = 0, CRASHED, NUM_ANIMS
};

void Stalactite::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/stalactite.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 8), glm::vec2(1.f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(ALIVE, 8);
	sprite->addKeyframe(ALIVE, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(CRASHED, 8);
	sprite->addKeyframe(CRASHED, glm::vec2(0.f, 0.5f));

	sprite->changeAnimation(ALIVE);

	status = DEAD;
	dyingTime = 16;
	alreadyDead = false;

	position = tileMapPos;
	startPosition = position;
	sprite->setPosition(position);
}

void Stalactite::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (status == ALIVE) {
		position.y += FALL_SPEED;
		if (map->collisionMoveDown(position, glm::ivec2(16, 1), &position.y)) {
			//position.y -= FALL_SPEED;
			sprite->changeAnimation(CRASHED);
			status = DYING;
		}
	}
	else if (status == DYING) {
		if (dyingTime == 0) {
			status = DEAD;
			alreadyDead = true;
		}
		else
			--dyingTime;
	}
	sprite->setPosition(position);
}

void Stalactite::setIdle()
{
	status = DEAD;
}

void Stalactite::setActive()
{
	if (!alreadyDead) {
		status = ALIVE;
		position = startPosition;
	}
}
