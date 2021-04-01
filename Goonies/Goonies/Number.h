#ifndef _NUMBER_INCLUDE
#define _NUMBER_INCLUDE

#include "Entity.h"

class Number : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void changeNumber(int n);
};

#endif // _NUMBER_INCLUDE