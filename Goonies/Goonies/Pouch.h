#ifndef _POUCH_INCLUDE
#define _POUCH_INCLUDE

#include "Entity.h"

class Pouch : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);

	void setIdle();
	void setActive();

	void interact();

	void setContent(int content);

private:
	int content;
	int flashTime;
	bool started;
	bool pickedUp;
};

#endif // _POUCH_INCLUDE