#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Entity.h"

void Entity::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
}

void Entity::update(int deltaTime)
{
}

void Entity::render()
{
	sprite->render();
}

void Entity::setPosition(const glm::vec2 & pos)
{
	position = pos;
	sprite->setPosition(position);
}
