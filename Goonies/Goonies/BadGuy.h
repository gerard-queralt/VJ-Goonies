#include "Entity.h"

class BadGuy: public Entity
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
};

