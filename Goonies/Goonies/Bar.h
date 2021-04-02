#ifndef _BAR_INCLUDE
#define _BAR_INCLUDE

#include "Entity.h"
#include "SubBar.h"

class Bar : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void render();
	void update(int deltaTime);

	void setType(int type, ShaderProgram &shaderProgram);

private:
	std::vector<SubBar*> bars;
	int type;
};

#endif // _BAR_INCLUDE