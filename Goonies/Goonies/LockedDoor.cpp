#include "LockedDoor.h"

void LockedDoor::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/lockeddoor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	status = ALIVE;

	position = tileMapPos;
	startPosition = position;
	sprite->setPosition(position);
}

void LockedDoor::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void LockedDoor::interact()
{
	glm::vec2 lockPosition = glm::vec2(position.x, position.y + 2 * map->getTileSize()); //el candau esta 2 tile per sota
	if (inContactWithPlayer(lockPosition, glm::vec2(8, 8))) {
		if (player->useKey()) {
			status = DEAD; //temporal
		}
	}
}
