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

