#ifndef _SKULL_INCLUDE
#define _SKULL_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Skull
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void setPosition(const glm::vec2 &pos);

private:
	bool movingLeft;
	glm::vec2 posSkull;
	Texture spritesheet;
	Sprite *sprite;
};

#endif // _SKULL_INCLUDE
