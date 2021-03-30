#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Entity.h"

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

