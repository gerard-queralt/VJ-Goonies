#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Entity.h"
#include "TileMap.h"

void Entity::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
}

void Entity::update(int deltaTime)
{
}

void Entity::render()
{
	if(status != DEAD)
		sprite->render();
}

void Entity::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}

void Entity::setIdle()
{
}

void Entity::setActive()
{
}

glm::vec2 Entity::getPosition()
{
	return position;
}

void Entity::setPlayer(Player * player)
{
	this->player = player;
}

void Entity::interact()
{
}

bool Entity::inContactWithPlayer(const glm::vec2 &entityPosition, const glm::vec2 &entitySize)
{
	glm::vec2 playerPos = player->getPosition();

	int x0, y0, x1, y1;

	y0 = playerPos.y - 6; //offset per ajustar la posicio, perque caminem atravessant una tile
	y1 = playerPos.y + 16; //altura personatge
	x0 = playerPos.x;
	x1 = playerPos.x + 16; //amplada personatge

	for (int y = y0; y <= y1; ++y) {
		for (int x = x0; x <= x1; ++x) {
			if (position.x <= x && x <= position.x + entitySize.x
				&& position.y <= y && y <= position.y + entitySize.y)
				return true;
		}
	}
	return false;
}

