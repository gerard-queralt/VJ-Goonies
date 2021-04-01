#include "Lock.h"

void Lock::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/lock.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	status = ALIVE;

	position = tileMapPos;
	startPosition = position;
	sprite->setPosition(position);
}

void Lock::update(int deltaTime)
{
	sprite->update(deltaTime);
}
