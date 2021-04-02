#include "Key.h"

#define FLASH_TIME 3

void Key::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/key.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	status = DEAD;
	flashTime = 0;
	started = false;
	pickedUp = false;

	position = tileMapPos;
	sprite->setPosition(position);
}

void Key::update(int deltaTime)
{
	if (started && !pickedUp) {
		if (flashTime == FLASH_TIME) {
			if (status == DEAD)
				status = ALIVE;
			else if (status == ALIVE)
				status = DEAD;
			flashTime = 0;
		}
		else
			++flashTime;
	}
	sprite->update(deltaTime);
}

void Key::setIdle()
{
	status = DEAD;
	started = false;
}

void Key::setActive()
{
	if (!pickedUp) {
		status = ALIVE;
		flashTime = 0;
		started = true;
	}
}

void Key::interact()
{
	if (inContactWithPlayer(position, glm::ivec2(8,8))) {
		if (player->getKey()) {
			status = DEAD;
			pickedUp = true;
		}
	}
}
