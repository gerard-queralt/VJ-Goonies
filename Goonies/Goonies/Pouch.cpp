#include "Pouch.h"

#define FLASH_TIME 16

enum Content
{
	HELMET = 0, HYPER_SHOES, WATERCOAT_GREY, WATERCOAT_BLUE, SPELLBOOK, POTION
};

void Pouch::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/pouch.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	status = DEAD;
	pickedUp = false;

	position = tileMapPos;
	sprite->setPosition(position);
}

void Pouch::update(int deltaTime)
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

void Pouch::setIdle()
{
	status = DEAD;
	started = false;
}

void Pouch::setActive()
{
	if (!pickedUp) {
		status = ALIVE;
		flashTime = 0;
		started = true;
	}
}

void Pouch::interact()
{
	if (!pickedUp && inContactWithPlayer(position, glm::ivec2(8, 8))) {
		switch (content)
		{
		case HELMET:
		case HYPER_SHOES:
		case WATERCOAT_GREY:
		case WATERCOAT_BLUE:
		case SPELLBOOK:
			player->givePowerUp(content);
			break;
		case POTION:
			player->heal(15);
			break;
		default:
			break;
		}
		status = DEAD;
		pickedUp = true;
	}
}

void Pouch::setContent(int content)
{
	this->content = content;
}
