#ifndef _KEY_INCLUDE
#define _KEY_INCLUDE

#include "Entity.h"
class Key : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);

	void setIdle();
	void setActive();

	void interact();

private:
	int flashTime;
	bool started;
	bool pickedUp;
};

#endif // _KEY_INCLUDE