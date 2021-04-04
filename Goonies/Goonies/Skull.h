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

	void setBehaviour(int bhv);

private:
	int behaviour;
	bool falling;

	bool movingLeft;
	int spawnTime;
	int deathTime;
	int dmgCD;
};

#endif // _SKULL_INCLUDE
