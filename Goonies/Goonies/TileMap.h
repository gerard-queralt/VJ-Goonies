#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"

//forward declarations
class Entity;
class Player;

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void update(int deltaTime);
	void render() const;
	void free();
		
	void setEntitiesIdle();
	void setEntitiesActive();
	void setEntitiesPlayer(Player *player);

	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::vec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::vec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::vec2 &pos, const glm::ivec2 &size, float *posY) const;
	bool collisionMoveDownEntities(const glm::vec2 & pos, const glm::ivec2 & size) const;
	bool collisionMoveUp(const glm::vec2 & pos, const glm::ivec2 & size) const;

	bool climbUp(const glm::vec2 &pos, const glm::ivec2 &size, float *posX, bool lookingLeft) const;
	bool climbDown(const glm::vec2 &pos, const glm::ivec2 &size, float *posX, bool lookingLeft) const;
	bool stopClimbing(const glm::vec2 &pos, const glm::ivec2 &size, bool up) const;

	void detectChangeScene(glm::vec2 &pos, const glm::ivec2 &size, int state) const;
	void detectChangeLevel(glm::vec2 &pos, const glm::ivec2 &size) const;

	bool waterfallCollision(const glm::vec2 &pos, const glm::ivec2 &size) const;

	void setLvl(int lvl);

	glm::vec2 getMapSize();
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

	bool notWalkable(int tile) const;

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;

	std::vector<Entity*> entities;
	int lvl;
};


#endif // _TILE_MAP_INCLUDE


