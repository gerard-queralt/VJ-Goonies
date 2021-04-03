#include "LockedDoor.h"
#include "Potion.h"
#include "Friend.h"

enum Contents {
	POTION = 0, FRIEND
};

enum Colors {
	RED = 0, BLUE, GREEN
};

void LockedDoor::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	string path = "images/lockeddoor_" + color + ".png";
	spritesheet.loadFromFile(path, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	status = ALIVE;

	position = tileMapPos;
	startPosition = position;
	sprite->setPosition(position);

	glm::vec2 contentPosition = glm::vec2(position.x + 2 * map->getTileSize(), position.y + 2 * map->getTileSize());

	if (content == POTION) {
		contentEntity = new Potion();
	}
	else if(content == FRIEND){
		contentEntity = new Friend();
	}
	contentEntity->init(contentPosition, shaderProgram);
	contentEntity->setTileMap(map);
}

void LockedDoor::setContent(int content)
{
	this->content = content;
}

void LockedDoor::setColor(int color)
{
	switch (color)
	{
	case RED:
		this->color = "red";
		break;
	case BLUE:
		this->color = "blue";
		break;
	case GREEN:
		this->color = "green";
		break;
	default:
		break;
	}
}

void LockedDoor::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void LockedDoor::render()
{
	if (status == ALIVE)
		sprite->render();
	else
		contentEntity->render();
}

void LockedDoor::interact()
{
	if (status == ALIVE) {
		glm::vec2 lockPosition = glm::vec2(position.x, position.y + 2 * map->getTileSize()); //el candau esta 2 tile per sota
		if (inContactWithPlayer(lockPosition, glm::ivec2(8, 8))) {
			if (player->useKey()) {
				open();
			}
		}
	}
	else
		contentEntity->interact();
}

void LockedDoor::open()
{
	status = DEAD;
	contentEntity->setPlayer(player);
	contentEntity->setActive();
}
