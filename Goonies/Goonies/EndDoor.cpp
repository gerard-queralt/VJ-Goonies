#include <GL/glew.h>
#include <GL/glut.h>
#include "EndDoor.h"
#include "Game.h"

void EndDoor::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/enddoor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	status = ALIVE;

	position = tileMapPos;
	position.x += map->getTileSize();
	sprite->setPosition(position);
}

void EndDoor::interact()
{
	if (status == ALIVE && player->getNumFriends() == 6) {
		status = DEAD;
	}
	if (status == DEAD && inContactWithPlayer(position, glm::ivec2(24, 32))) {
		if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			Game::instance().changeScene(610216);
	}
}
