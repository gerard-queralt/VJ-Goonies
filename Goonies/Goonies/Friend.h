#ifndef _FRIEND_INCLUDE
#define _FRIEND_INCLUDE

#include "Entity.h"

class Friend : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);

	void setIdle();
	void setActive();

	void interact();
};

#endif // _SKULL_INCLUDE