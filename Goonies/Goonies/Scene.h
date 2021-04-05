#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"

#include "Number.h"
#include "Bar.h"
#include "Key.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{
public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

	void startGame();
	void changeScene(int code);
	void gameOver();
	void endGame();

private:
	void initShaders();
	void createGame();

	void changeLevel(int lvl);

	void setUpUISprites();
	void renderUI();

private:
	TileMap *map[6][3];
	int currentScene;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	int currentState;
	Texture titleimage;
	Sprite *title;
	int startLevelTime;
	Texture instructionsimage;
	Sprite *instructions;
	Texture gameoverimage;
	Sprite *gameover;
	Texture endgameimage;
	Sprite *endgame;
	Texture creditsimage;
	Sprite *credits;

	Texture UIimage;
	Sprite *prerenderedUI;
	Number *lvlNumber;
	Number *scenenNumber;
	Bar *hpBar;
	Bar *expBar;
	Key *keyUI;
	bool currentKeyState; //perque volem que el setIdle i setActive nomes s'execute quan canvi l'estat de la clau del personatge
	//sprites UI
	std::vector<Sprite*> powerUpsUI;
	Texture powerUpsSpritesheet;
	std::vector<Sprite *> friends;
	Texture friendSpritesheet;
	Sprite *godmode;
	Texture godmodeSpritesheet;
};


#endif // _SCENE_INCLUDE

