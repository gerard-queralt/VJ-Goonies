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

	status = ALIVE;
	startTime = 60; //temporal
	dyingTime = 16;

	position = tileMapPos;
	sprite->setPosition(position);
}

void Stalactite::update(int deltaTime)
{
	if (startTime == 0) {
		sprite->update(deltaTime);
		if (status == ALIVE) {
			position.y += FALL_SPEED;
			if (map->collisionMoveDown(position, glm::ivec2(16, 8), &position.y)) {
				//position.y -= FALL_SPEED;
				sprite->changeAnimation(CRASHED);
				status = DYING;
			}
		}
		else if (status == DYING) {
			if (dyingTime == 0)
				status = DEAD;
			else
				--dyingTime;
		}
		sprite->setPosition(position);
	}
	else
		--startTime;
}
