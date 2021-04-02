#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"

#include "Number.h"
#include "Bar.h"
#include "Key.h"
#include "Helmet.h"


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

private:
	void initShaders();

	void createLevel(int lvl);

private:
	TileMap *map[3];
	int currentScene;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	int currentState;
	Texture titleimage;
	Sprite *title;
	int startLevelTime;

	Texture UIimage;
	Sprite *prerenderedUI;
	Number *lvlNumber;
	Number *scenenNumber;
	Bar *hpBar;
	Bar *expBar;
	Key *keyUI;
	bool currentKeyState; //perque volem que el setIdle i setActive nomes s'execute quan canvi l'estat de la clau del personatge
	Helmet *helmetUI;
};


#endif // _SCENE_INCLUDE

