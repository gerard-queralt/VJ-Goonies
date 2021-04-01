#ifndef _POTION_INCLUDE
#define _POTION_INCLUDE

#include "Entity.h"

class Potion : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);

	void setIdle();
	void setActive();

	void interact();
};

#endif // _POTION_INCLUDE