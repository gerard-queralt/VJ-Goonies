#include <iostream>
#include "Skull.h"

#define SPEED 1

enum SkullAnims
{
	MOVE_LEFT = 0, MOVE_RIGHT, SPAWNING, DYING, NUM_ANIM
};

void Skull::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
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

	sprite->changeAnimation(SPAWNING);
	movingLeft = true;

	status = SPAWNING;
	spawnTime = 60;
}

void Skull::update(int deltaTime)
{
	if (status == ALIVE) {
		sprite->update(deltaTime);
		if (movingLeft) {
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			position.x -= SPEED;
			if (position.x < 1 * 8) {
				position.x += SPEED;
				movingLeft = false;
			}
		}
		else {
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			position.x += SPEED;
			if (position.x > 30 * 8) {
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
}
