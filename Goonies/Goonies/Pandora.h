#ifndef _PANDORA_INCLUDE
#define _PANDORA_INCLUDE

#include "Entity.h"
#include "PandoraBubble.h"

class Pandora : public Entity
{
public:
	void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setIdle();
	void setActive();

	void interact();

	void setPlayer(Player *player);

private:
	void randomize();

private:
	std::vector<PandoraBubble*> bubbles;

	int hp;

	int invisTime;

	bool movingLeft;
	bool movingUp;

	int dmgCD;
	int hurtCD;

	bool flashing;
	int flashTime;
	int flashingTime;
	int randomizeTime;

	int victoryLap;
};

#endif // _PANDORA_INCLUDE