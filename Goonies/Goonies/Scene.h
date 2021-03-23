#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Skull.h"


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

	void changeState();

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Skull *skull;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	int currentState;
	Texture titleimage;
	Sprite *title;
};


#endif // _SCENE_INCLUDE

