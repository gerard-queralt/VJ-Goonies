#ifndef _ENDDOOR_INCLUDE
#define _ENDDOOR_INCLUDE

#include "Entity.h"

class EndDoor : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);

	void interact();
};

#endif // _LOCKEDDOOR_INCLUDE