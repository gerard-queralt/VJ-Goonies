#ifndef _PANDORABUBBLE_INCLUDE
#define _PANDORABUBBLE_INCLUDE

#include "Entity.h"

class PandoraBubble : public Entity
{
	enum Movement {
		STAY = 0, POSITIVE, NEGATIVE
	};

public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);

	void interact();

	void randomize();

private:
	float speed;

	int horizontal;
	int vertical;

	int dmgCD;
};

#endif // _PANDORABUBBL_INCLUDE