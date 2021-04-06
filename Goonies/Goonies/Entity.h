#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"

class Entity
{
public:
	virtual void init(const glm::vec2 &tileMapPos, ShaderProgram &shaderProgram);
	virtual void update(int deltaTime);
	virtual void render();
	void setTileMap(TileMap *tileMap);

	virtual void setIdle();
	virtual void setActive();

	glm::vec2 getPosition();

	virtual void setPlayer(Player *player);
	virtual void interact();

protected:
	bool inContactWithPlayer(const glm::vec2 &entityPosition, const glm::ivec2 &entitySize);

	bool hitByPunch(const glm::vec2 &entityPosition, const glm::ivec2 &entitySize);

	enum EntityStatus {
		ALIVE = 0, DEAD, SPAWNING, DYING
	};

	EntityStatus status;
	glm::vec2 position, startPosition;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player *player;
};

#endif // _ENTITY_INCLUDE