#include "Number.h"

void Number::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.125f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(7);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(1, 8);
	sprite->addKeyframe(1, glm::vec2(0.125f, 0.f));

	sprite->setAnimationSpeed(2, 8);
	sprite->addKeyframe(2, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(3, 8);
	sprite->addKeyframe(3, glm::vec2(0.375f, 0.f));

	sprite->setAnimationSpeed(4, 8);
	sprite->addKeyframe(4, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(5, 8);
	sprite->addKeyframe(5, glm::vec2(0.625f, 0.f));

	sprite->setAnimationSpeed(6, 8);
	sprite->addKeyframe(6, glm::vec2(0.75f, 0.f));

	status = ALIVE;
	position = tileMapPos;
	sprite->setPosition(position);
}

void Number::changeNumber(int n)
{
	sprite->changeAnimation(n);
}
