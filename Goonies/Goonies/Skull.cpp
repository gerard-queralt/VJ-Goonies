#include <iostream>
#include "Skull.h"

enum SkullAnims
{
	MOVE_LEFT = 0, MOVE_RIGHT, NUM_ANIM
};

void Skull::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/skull.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(12, 12), glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIM);

	sprite->setAnimationSpeed(MOVE_RIGHT, 7);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(MOVE_LEFT, 7);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(MOVE_LEFT);
	movingLeft = true;

	status = ALIVE;
}

void Skull::update(int deltaTime)
{
	if (status == ALIVE) {
		sprite->update(deltaTime);
		if (movingLeft) {
			position.x -= 2;
			if (position.x < 1 * 8) {
				position.x += 2;
				movingLeft = false;
				sprite->changeAnimation(MOVE_RIGHT);
			}
		}
		else {
			position.x += 2;
			if (position.x > 30 * 8) {
				position.x -= 2;
				movingLeft = true;
				sprite->changeAnimation(MOVE_LEFT);
			}
		}
		sprite->setPosition(position);
	}
}
