#include "StartDoor.h"

void StartDoor::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/startdoor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	status = ALIVE;
	startTime = 60;
}

void StartDoor::update(int deltaTime)
{
	if (status == ALIVE) {
		--startTime;
		if (startTime == 0)
			status = DEAD;
	}
}
