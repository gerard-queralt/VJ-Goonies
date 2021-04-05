#include <iostream>
#include "WaterDrop.h"

#define FALL_SPEED 1
#define CHANGE_STATE_TIME 16
#define CYCLE_TIME 60

enum WaterDropAnim
{
	ROOF = 0, DROPPING, FALLING, SPLASHED, NUM_ANIM
};

void WaterDrop::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/waterdrop.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIM);

	sprite->setAnimationSpeed(ROOF, 8);
	sprite->addKeyframe(ROOF, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(DROPPING, 8);
	sprite->addKeyframe(DROPPING, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(FALLING, 8);
	sprite->addKeyframe(FALLING, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(SPLASHED, 8);
	sprite->addKeyframe(SPLASHED, glm::vec2(0.5f, 0.5f));

	sprite->changeAnimation(ROOF);

	status = SPAWNING;
	changeStateTime = CHANGE_STATE_TIME;
	cycleTime = 0; 

	position = tileMapPos;
	startPosition = position;
	sprite->setPosition(position);
}

void WaterDrop::update(int deltaTime)
{
	if (cycleTime == CYCLE_TIME) {
		if (status == DEAD)
			status = ALIVE;
		sprite->update(deltaTime);
		if (sprite->animation() == ROOF) {
			if (changeStateTime == 0) {
				sprite->changeAnimation(DROPPING);
				changeStateTime = CHANGE_STATE_TIME;
			}
			else
				--changeStateTime;
		}
		else if (sprite->animation() == DROPPING) {
			if (changeStateTime == 0) {
				sprite->changeAnimation(FALLING);
				changeStateTime = CHANGE_STATE_TIME;
			}
			else
				--changeStateTime;
		}
		else if (sprite->animation() == FALLING) {
			position.y += FALL_SPEED;
			if (inContactWithPlayer(position, glm::ivec2(4, 2))) {
				sprite->changeAnimation(SPLASHED);
				position.y += 2;
				if(!player->getPowerUps()[2] /*grey watercoat*/)
					player->hurt(5);
			}
			else if (map->collisionMoveDown(position, glm::ivec2(4, 2), &position.y)) {
				sprite->changeAnimation(SPLASHED);
			}
		}
		else {
			if (changeStateTime == 0) {
				status = DEAD;
				cycleTime = 0;
				changeStateTime = CHANGE_STATE_TIME;
				sprite->changeAnimation(ROOF);
				position = startPosition;
			}
			else
				--changeStateTime;
		}
		sprite->setPosition(position);
	}
	else
		++cycleTime;
}

void WaterDrop::setIdle()
{
	status = DEAD;
}

void WaterDrop::setActive()
{
	status = DEAD;
	cycleTime = 0;
	position = startPosition;
	sprite->setPosition(position);
	sprite->changeAnimation(ROOF);
}
