#include <iostream>
#include "Stalactite.h"

#define FALL_SPEED 1

enum StalAnims
{
	ALIVE = 0, CRASHED, NUM_ANIMS
};

enum Colors {
	RED = 0, BLUE, GREEN
};

void Stalactite::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	string path = "images/stalactite_" + color + ".png";
	spritesheet.loadFromFile(path, TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 8), glm::vec2(1.f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIMS);

	sprite->setAnimationSpeed(ALIVE, 8);
	sprite->addKeyframe(ALIVE, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(CRASHED, 8);
	sprite->addKeyframe(CRASHED, glm::vec2(0.f, 0.5f));

	sprite->changeAnimation(ALIVE);

	status = SPAWNING;
	dyingTime = 16;
	alreadyDead = false;

	position = tileMapPos;
	startPosition = position;
	sprite->setPosition(position);
}

void Stalactite::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (status == ALIVE) {
		position.y += FALL_SPEED;
		if (inContactWithPlayer(glm::vec2(position.x + 6, position.y), glm::ivec2(4, 1))) {
			if(!player->getPowerUps()[0] /*helmet*/)
				player->hurt(4);
			status = DEAD;
			alreadyDead = true;
		}
		else if (map->collisionMoveDown(position, glm::ivec2(16, 1), &position.y)) {
			sprite->changeAnimation(CRASHED);
			status = DYING;
		}
	}
	else if (status == DYING) {
		if (dyingTime == 0) {
			status = DEAD;
			alreadyDead = true;
		}
		else
			--dyingTime;
	}
	else if (status == SPAWNING) {
		if (playerClose())
			status = ALIVE;
	}
	sprite->setPosition(position);
}

void Stalactite::setIdle()
{
	if (!alreadyDead)
		status = SPAWNING;
	else
		status = DEAD;
}

void Stalactite::setActive()
{
	if (!alreadyDead) {
		status = SPAWNING;
		position = startPosition;
	}
}

void Stalactite::setColor(int color)
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

bool Stalactite::playerClose()
{
	glm::vec2 playerPos = player->getPosition();

	int x0, y0, x1, y1;

	y0 = position.y;
	y1 = position.y + 7 * map->getTileSize(); //aproximat, segurament millorable
	x0 = position.x;
	x1 = position.x + 2 * map->getTileSize(); //una tile despres

	for (int y = y0; y <= y1; ++y) {
		for (int x = x0; x <= x1; ++x) {
			if (playerPos.x <= x && x <= playerPos.x + 16 /*player size*/
				&& playerPos.y <= y && y <= playerPos.y + 16 /*player height*/)
				return true;
		}
	}
	return false;
}
