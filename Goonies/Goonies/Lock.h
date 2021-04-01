#ifndef _LOCK_INCLUDE
#define _LOCK_INCLUDE

#include "Entity.h"

class Lock : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
};

#endif // _LOCK_INCLUDE