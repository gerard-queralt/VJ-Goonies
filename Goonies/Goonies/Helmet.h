#ifndef _HELMET_INCLUDE
#define _HELMET_INCLUDE

#include "Entity.h"

class Helmet : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	//void update(int deltaTime);

	void setIdle();
	void setActive();

	//void interact();
};

#endif // _HELMET_INCLUDE