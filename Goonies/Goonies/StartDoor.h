#ifndef _STARTDOOR_INCLUDE
#define _STARTDOOR_INCLUDE

#include "Entity.h"

class StartDoor: public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);

private:
	int startTime;
};

#endif // _STARTDOOR_INCLUDE