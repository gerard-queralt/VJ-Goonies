#include "LockedDoor.h"

void LockedDoor::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/lockeddoor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	status = ALIVE;

	position = tileMapPos;
	position.y += map->getTileSize(); //perque al fitxer de text esta un tile mes amunt
	startPosition = position;
	sprite->setPosition(position);
}

void LockedDoor::update(int deltaTime)
{
	sprite->update(deltaTime);
}
