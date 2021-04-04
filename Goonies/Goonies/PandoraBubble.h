#ifndef _PANDORABUBBLE_INCLUDE
#define _PANDORABUBBLE_INCLUDE

#include "Entity.h"

class PandoraBubble : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);

	void setIdle();
	void setActive();

	void interact();

	void randomize();

private:
	float speed;

	bool movingLeft;
	bool movingUp;

	int dmgCD;
};

#endif // _PANDORABUBBL_INCLUDE