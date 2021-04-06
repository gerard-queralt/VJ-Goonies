#include "Potion.h"

void Potion::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/vitpot.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	status = DEAD;

	position = tileMapPos;
	sprite->setPosition(position);
}

void Potion::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Potion::setIdle()
{
}

void Potion::setActive()
{
	status = ALIVE;
}

void Potion::interact()
{
	if (status == ALIVE && inContactWithPlayer(position, glm::ivec2(8, 8))) {
		player->heal(10);
		status = DEAD;
	}
}
