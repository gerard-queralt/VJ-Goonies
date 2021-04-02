#ifndef _SUBBAR_INCLUDE
#define _SUBBAR_INCLUDE

#include "Entity.h"

class SubBar : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);

	void setType(int type);
	void setAnimation(int anim);

private:
	float heightInSpritesheet;
};

#endif // _SUBBAR_INCLUDE