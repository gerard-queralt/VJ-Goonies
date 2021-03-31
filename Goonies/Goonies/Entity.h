#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Entity
{
public:
	virtual void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	virtual void update(int deltaTime);
	void render();
	void setTileMap(TileMap *tileMap);

	virtual void setIdle();
	virtual void setActive();

protected:
	enum EntityStatus {
		ALIVE = 0, DEAD, SPAWNING, DYING
	};

	EntityStatus status;
	glm::vec2 position, startPosition;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};

#endif // _ENTITY_INCLUDE