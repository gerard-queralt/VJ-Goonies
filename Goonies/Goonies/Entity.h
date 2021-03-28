#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Entity
{
public:
	virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	virtual void update(int deltaTime);
	void render();
	void setPosition(const glm::vec2 &pos);

protected:
	enum EntityStatus {
		ALIVE = 0, DEAD, SPAWNING, DYING
	};

	EntityStatus status;
	glm::vec2 position;
	Texture spritesheet;
	Sprite *sprite;
};

#endif // _ENTITY_INCLUDE