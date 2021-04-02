#include "Helmet.h"

void Helmet::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/helmet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	status = DEAD;

	position = tileMapPos;
	sprite->setPosition(position);
}

void Helmet::setIdle()
{
	status = DEAD;
}

void Helmet::setActive()
{
	status = ALIVE;
}
