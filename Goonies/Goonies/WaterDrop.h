#ifndef _WATERDROP_INCLUDE
#define _WATERDROP_INCLUDE

#include "Entity.h"

class WaterDrop : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void setIdle();
	void setActive();

private:
	int cycleTime;
	int changeStateTime;
};

#endif // _WATERDROP_INCLUDE