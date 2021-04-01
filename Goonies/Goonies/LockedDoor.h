#ifndef _LOCKEDDOOR_INCLUDE
#define _LOCKEDOOR_INCLUDE

#include "Entity.h"

class LockedDoor : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);

	void interact();
};

#endif // _LOCKEDDOOR_INCLUDE