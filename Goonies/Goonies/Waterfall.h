#ifndef _WATERWALL_INCLUDE
#define _WATERWALL_INCLUDE

#include "Entity.h"

class Waterfall : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	//void update(int deltaTime);
	void render();
	
	void setIdle();
	void setActive();

	void interact();

private:
	ShaderProgram shaderProgram; //necessari per iniciar les sprites del mig

	std::vector<Sprite*> middle;
	Sprite *top;
	
	int fallTime;
	int secondFallTime;
	int stayTime;
	int stayAnimationTime;
	int cycleTime;
};

#endif // _WATERWALL_INCLUDE