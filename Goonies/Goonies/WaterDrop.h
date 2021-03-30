#ifndef _WATERDROP_INCLUDE
#define _WATERDROP_INCLUDE

#include "Entity.h"

class WaterDrop : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);

private:
	int startTime; //temporal
	int changeStateTime;
	glm::vec2 startPos;
};

#endif // _WATERDROP_INCLUDE