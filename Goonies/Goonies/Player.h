#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

	enum PlayerStatus {
		GROUNDED = 0, JUMPING, FALLING, PUNCHING, CLIMBING
	};

	enum SpaceKeyStatus {
		PRESSED = 0, RELEASED, HOLD
	};

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPosition();

	void heal(int heal);
	void hurt(int dmg);

	bool getKey();
	bool useKey();
	
private:
	int health;
	int level;
	int exp;
	bool powerUps;
	bool hasKey;

	bool lookingLeft; //Boolea per recuperar l'orientacio despres d'escalar

	PlayerStatus status;
	SpaceKeyStatus spaceKeyStatus;
	int startTime;
	int punchingTime;
	bool keepMovingInAir;
	glm::ivec2 tileMapDispl;
	glm::vec2 posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE


