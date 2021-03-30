#ifndef _STALACTITE_INCLUDE
#define _STALACTITE_INCLUDE

#include "Entity.h"

class Stalactite : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);

private:
	int startTime; //valor temporal mentres no fem la deteccio del personatge
	int dyingTime;
};

#endif // _STALACTITE_INCLUDE