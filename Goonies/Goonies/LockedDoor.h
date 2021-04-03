#ifndef _LOCKEDDOOR_INCLUDE
#define _LOCKEDOOR_INCLUDE

#include "Entity.h"

class LockedDoor : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void interact();

	void setContent(int content);
	void setColor(int color);

private:
	void open();

	int content;
	string color;

	Entity *contentEntity;
};

#endif // _LOCKEDDOOR_INCLUDE