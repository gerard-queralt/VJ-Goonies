#ifndef _DOUBLELOCKEDDOOR_INCLUDE
#define _DOUBLELOCKEDOOR_INCLUDE

#include "Entity.h"
#include "Friend.h"

class DoubleLockedDoor : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void interact();

private:
	int locks;
	void open();

	Friend *content;

	Texture blackSquareSpritesheet;
	Sprite *blackSquare; //per tapar un candau
};

#endif // _DOUBLELOCKEDDOOR_INCLUDE