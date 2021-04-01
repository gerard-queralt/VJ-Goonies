#ifndef _STALACTITE_INCLUDE
#define _STALACTITE_INCLUDE

#include "Entity.h"

class Stalactite : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	
	void setIdle();
	void setActive();

private:
	bool playerClose();

	int dyingTime;
	bool alreadyDead; //les estalactites no respawnegen

};

#endif // _STALACTITE_INCLUDE