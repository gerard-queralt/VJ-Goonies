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

	enum KeyStatus {
		PRESSED = 0, RELEASED, HOLD
	};

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	int getStatus();
	glm::vec2 getPosition();

	void heal(int heal);
	void hurt(int dmg);

	void gainXP(int exp);

	int getHealth();
	int getExp();
	bool getHasKey();

	bool getKey();
	bool useKey();
	
	void givePowerUp(int pwup);

	std::vector<bool> getPowerUps();

	void rescueFriend();
	int getNumFriends();

	void switchGodMode();

	bool getGodMode();

	void resetStartTime(int time);

	bool getLookingLeft();

private:
	void checkSpecialCommands();

	int health;
	int exp;
	std::vector<bool> powerUps;
	bool hasKey;
	int numFriends;

	bool godMode;

	bool lookingLeft; //Boolea per recuperar l'orientacio despres d'escalar

	PlayerStatus status;
	KeyStatus spaceKeyStatus;
	int startTime;
	int punchingTime;
	bool keepMovingInAir;
	glm::ivec2 tileMapDispl;
	glm::vec2 posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

	//per a pampallugues
	bool flashing;
	int flashingTime; //temps fent pampallugues
	int flashTime; //temps entre pampallugues

	//per a comandes
	KeyStatus gKeyStatus;
	KeyStatus fKeyStatus;
	KeyStatus oneKeyStatus;
	KeyStatus twoKeyStatus;
	KeyStatus threeKeyStatus;
	KeyStatus fourKeyStatus;
	KeyStatus fiveKeyStatus;
	int lvlTP;
	int sceneTP;
	KeyStatus sixKeyStatus;
	KeyStatus sevenKeyStatus;
	KeyStatus eightKeyStatus;
	KeyStatus nineKeyStatus;
	KeyStatus zeroKeyStatus;
};


#endif // _PLAYER_INCLUDE


