#ifndef _SKULL_INCLUDE
#define _SKULL_INCLUDE

#include "Entity.h"

class Skull : public Entity
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);

private:
	bool movingLeft;
	int spawnTime;
};

#endif // _SKULL_INCLUDE
