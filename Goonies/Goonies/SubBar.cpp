#include "SubBar.h"

enum Type {
	HEALTH, EXP
};

void SubBar::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/bars.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.1f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(9);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.8f, 0.f));

	sprite->setAnimationSpeed(1, 8);
	sprite->addKeyframe(1, glm::vec2(0.f, heightInSpritesheet));

	sprite->setAnimationSpeed(2, 8);
	sprite->addKeyframe(2, glm::vec2(0.1f, heightInSpritesheet));

	sprite->setAnimationSpeed(3, 8);
	sprite->addKeyframe(3, glm::vec2(0.2f, heightInSpritesheet));

	sprite->setAnimationSpeed(4, 8);
	sprite->addKeyframe(4, glm::vec2(0.3f, heightInSpritesheet));

	sprite->setAnimationSpeed(5, 8);
	sprite->addKeyframe(5, glm::vec2(0.4f, heightInSpritesheet));

	sprite->setAnimationSpeed(6, 8);
	sprite->addKeyframe(6, glm::vec2(0.5f, heightInSpritesheet));

	sprite->setAnimationSpeed(7, 8);
	sprite->addKeyframe(7, glm::vec2(0.6f, heightInSpritesheet));

	sprite->setAnimationSpeed(8, 8);
	sprite->addKeyframe(8, glm::vec2(0.7f, heightInSpritesheet));

	sprite->changeAnimation(8);
	status = ALIVE;

	position = tileMapPos;
	sprite->setPosition(position);
}

void SubBar::setType(int type)
{
	if (type == HEALTH)
		heightInSpritesheet = 0;
	else if (type == EXP)
		heightInSpritesheet = 0.5;
}

void SubBar::setAnimation(int anim)
{
	sprite->changeAnimation(anim);
}
