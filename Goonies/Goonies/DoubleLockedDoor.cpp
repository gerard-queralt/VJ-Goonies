#include "DoubleLockedDoor.h"

enum Colors {
	RED, GREEN
};

void DoubleLockedDoor::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	string path = "images/doublelockeddoor_" + color + ".png";
	spritesheet.loadFromFile(path, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	status = ALIVE;
	locks = 2;

	position = tileMapPos;
	startPosition = position;
	sprite->setPosition(position);

	glm::vec2 contentPosition = glm::vec2(position.x + 2 * map->getTileSize(), position.y + 2 * map->getTileSize());

	content = new Friend();
	content->init(contentPosition, shaderProgram);
	content->setTileMap(map);

	blackSquareSpritesheet.loadFromFile("images/blacksquare.png", TEXTURE_PIXEL_FORMAT_RGBA);
	blackSquare = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(1.f, 1.f), &blackSquareSpritesheet, &shaderProgram);
	blackSquare->setNumberAnimations(0);
	blackSquare->setPosition(glm::vec2(position.x, position.y + 1 * map->getTileSize()));
}

void DoubleLockedDoor::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void DoubleLockedDoor::render()
{
	if (status == ALIVE) {
		sprite->render();
		if (locks == 1)
			blackSquare->render();
	}
	else
		content->render();
}

void DoubleLockedDoor::interact()
{
	if (status == ALIVE) {
		glm::vec2 lockPosition = glm::vec2(position.x, position.y + 2 * map->getTileSize()); //el candau esta 2 tile per sota
		if (inContactWithPlayer(lockPosition, glm::ivec2(8, 8))) {
			if (player->useKey()) {
				--locks;
				if (locks == 0)
					open();
			}
		}
	}
	else
		content->interact();
}

void DoubleLockedDoor::setColor(int color)
{
	switch (color)
	{
	case RED:
		this->color = "red";
		break;
	case GREEN:
		this->color = "green";
		break;
	default:
		break;
	}
}

void DoubleLockedDoor::open()
{
	status = DEAD;
	content->setPlayer(player);
	content->setActive();
}
