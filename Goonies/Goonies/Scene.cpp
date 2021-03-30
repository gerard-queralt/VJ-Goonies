#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#include "Sprite.h"

//Vores negres de la pantalla
#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 12.5
#define INIT_PLAYER_Y_TILES 7.5

enum GameStates
{
	MENU, PLAYING
};


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	titleimage.loadFromFile("images/title.png", TEXTURE_PIXEL_FORMAT_RGBA);
	title = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &titleimage, &texProgram);
	title->setPosition(glm::vec2(0.f, 0.f));
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	currentState = MENU;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (currentState != MENU) {
		map->update(deltaTime);
		player->update(deltaTime);
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	switch (currentState)
	{
	case MENU:
		title->render();
		break;
	case PLAYING:
		map->render();
		player->render();
		break;
	default:
		break;
	}

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
}

void Scene::startGame()
{
	if (currentState == MENU) {
		currentState = PLAYING;
		map = TileMap::createTileMap("levels/level01/scene01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		player->setTileMap(map);
		glm::vec2 mapSize = map->getMapSize();
		projection = glm::ortho(0.f, mapSize.x * 8.f, mapSize.y * 8.f + 16.f, -16.f);
	}
}

//Code es un numero de 6 xifres ABXXYY, on A es el nivell, B es l'escena
//i XX i YY la posicio de la tile on es coloca el personatge
void Scene::changeScene(int code)
{
	int lvl = code / 100000;
	int scene = (code / 10000) % 10;
	string path = std::string("levels/level0") + std::to_string(lvl) + std::string("/scene0") + std::to_string(scene) + std::string(".txt");
	map = TileMap::createTileMap(path, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	float x = (code % 10000) / 100;
	float y = (code % 100);
	y += 0.5; //per que no quedi elevat
	player->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	player->setTileMap(map);
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



