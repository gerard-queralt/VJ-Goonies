#include <iostream>
#include "WaterDrop.h"

#define FALL_SPEED 1
#define CHANGE_STATE_TIME 16

enum WaterDropAnim
{
	ROOF = 0, DROPPING, FALLING, SPLASHED, NUM_ANIM
};

void WaterDrop::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/waterdrop.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
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

	status = DEAD;
	changeStateTime = CHANGE_STATE_TIME;
	startTime = 60; //temporal

	position = tileMapPos;
	sprite->setPosition(position);
	startPos = position;
}

void WaterDrop::update(int deltaTime)
{
	if (startTime == 0) {
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
			if (position.y >= 5 * 8 + 9 * 8 - 2) { //aixo s'ha de canviar per deteccio de col·lisions
				position.y = 5 * 8 + 9 * 8 - 2;
				sprite->changeAnimation(SPLASHED);
			}
		}
		else {
			if (changeStateTime == 0) {
				status = DEAD;
				startTime = 60;
				changeStateTime = CHANGE_STATE_TIME;
				sprite->changeAnimation(ROOF);
				position = startPos;
			}
			else
				--changeStateTime;
		}
		sprite->setPosition(position);
	}
	else
		--startTime;
}
