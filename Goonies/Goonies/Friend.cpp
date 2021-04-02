#include "Friend.h"

void Friend::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/friend.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(8, 16), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	status = DEAD;

	position = tileMapPos;
	sprite->setPosition(position);
}

void Friend::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Friend::setIdle()
{
}

void Friend::setActive()
{
	status = ALIVE;
}

void Friend::interact()
{
	if (status == ALIVE && inContactWithPlayer(position, glm::ivec2(8, 8))) {
		player->rescueFriend();
		status = DEAD;
	}
}
