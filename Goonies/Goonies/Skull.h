#ifndef _SKULL_INCLUDE
#define _SKULL_INCLUDE

#include "Entity.h"

class Skull : public Entity
{

public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	
	void setIdle();
	void setActive();

private:
	bool movingLeft;
	int spawnTime;
	int deathTime;
};

#endif // _SKULL_INCLUDE
