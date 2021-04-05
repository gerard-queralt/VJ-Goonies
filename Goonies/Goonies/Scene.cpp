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
#define INIT_PLAYER_Y_TILES 9.5

#define START_LEVEL_TIME 60

#define NUM_POWER_UPS 5

enum GameStates
{
	MENU, LEVEL1, LEVEL2, LEVEL3, LEVEL4, LEVEL5, BOSS, INSTRUCTIONS, GAME_OVER, END_GAME, CREDITS
};


Scene::Scene()
{
	for (int i = 0; i < 6; ++i) {
		map[i][0] = map[i][1] = map[i][2] = NULL;
	}
	player = NULL;
}

Scene::~Scene()
{
	if (map != NULL){
		for (int i = 0; i < 6; ++i) {
			for (int j = 0; j < 3; ++j) {
				delete map[i][j];
			}
		}
	}
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	titleimage.loadFromFile("images/title.png", TEXTURE_PIXEL_FORMAT_RGBA);
	title = Sprite::createSprite(glm::ivec2(32.f * 8.f, 24.f * 8.f), glm::vec2(1.f, 1.f), &titleimage, &texProgram);
	title->setPosition(glm::vec2(0.f, 0.f));
	instructionsimage.loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructions = Sprite::createSprite(glm::ivec2(32.f * 8.f, 24.f * 8.f), glm::vec2(1.f, 1.f), &instructionsimage, &texProgram);
	instructions->setPosition(glm::vec2(0.f, 0.f));
	gameoverimage.loadFromFile("images/gameover.png", TEXTURE_PIXEL_FORMAT_RGBA);
	gameover = Sprite::createSprite(glm::ivec2(32.f * 8.f, 24.f * 8.f), glm::vec2(1.f, 1.f), &gameoverimage, &texProgram);
	gameover->setPosition(glm::vec2(0.f, 0.f));
	endgameimage.loadFromFile("images/theend.png", TEXTURE_PIXEL_FORMAT_RGBA);
	endgame = Sprite::createSprite(glm::ivec2(32.f * 8.f, 24.f * 8.f), glm::vec2(1.f, 1.f), &endgameimage, &texProgram);
	endgame->setPosition(glm::vec2(0.f, 0.f));
	creditsimage.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	credits = Sprite::createSprite(glm::ivec2(32.f * 8.f, 24.f * 8.f), glm::vec2(1.f, 1.f), &creditsimage, &texProgram);
	credits->setPosition(glm::vec2(0.f, 0.f));
	projection = glm::ortho(0.f, 32.f * 8.f, 24.f * 8.f, 0.f);
	currentTime = 0.0f;
	currentState = MENU;
	Game::instance().playSound("gameMenu", true);
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (currentState != MENU && currentState != INSTRUCTIONS) {
		if (currentState != GAME_OVER && currentState != END_GAME && currentState != CREDITS) {
			if (startLevelTime == 0) {
				map[currentState - 1][currentScene - 1]->setEntitiesActive();
			}
			--startLevelTime;
			map[currentState - 1][currentScene - 1]->update(deltaTime);
			player->update(deltaTime);
			hpBar->update(deltaTime);
			expBar->update(deltaTime);
			if (currentKeyState != player->getHasKey()) {
				if (player->getHasKey()) {
					keyUI->setActive();
				}
				else
					keyUI->setIdle();
				currentKeyState = player->getHasKey();
			}
		}
		keyUI->update(deltaTime);
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
	case LEVEL1:
	case LEVEL2:
	case LEVEL3:
	case LEVEL4:
	case LEVEL5:
	case BOSS:
		map[currentState-1][currentScene-1]->render();
		player->render();
		renderUI();
		break;
	case INSTRUCTIONS:
		instructions->render();
		break;
	case GAME_OVER:
		gameover->render();
		renderUI();
		break;
	case END_GAME:
		endgame->render();
		renderUI();
		break;
	case CREDITS:
		credits->render();
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
		currentState = INSTRUCTIONS;
	}
	else if (currentState == INSTRUCTIONS) {
		currentState = LEVEL1;
		player = new Player();
		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

		createGame();

		lvlNumber = new Number();
		lvlNumber->init(glm::vec2(27 * 8.f, 1 * 8.f), texProgram);

		currentScene = 1;
		changeLevel(1);

		scenenNumber = new Number();
		scenenNumber->init(glm::vec2(30 * map[0][0]->getTileSize(), 1 * map[0][0]->getTileSize()), texProgram);
		scenenNumber->changeNumber(1);

		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map[0][0]->getTileSize(), INIT_PLAYER_Y_TILES * map[0][0]->getTileSize()));
		player->setTileMap(map[0][0]);
		glm::vec2 mapSize = map[0][0]->getMapSize();
		startLevelTime = START_LEVEL_TIME;

		UIimage.loadFromFile("images/prerenderedui.png", TEXTURE_PIXEL_FORMAT_RGBA);
		prerenderedUI = Sprite::createSprite(glm::ivec2(mapSize.x * map[0][0]->getTileSize(), mapSize.y * map[0][0]->getTileSize()), glm::vec2(1.f, 1.f), &UIimage, &texProgram);
		prerenderedUI->setPosition(glm::vec2(0.f, 0.f));
		
		hpBar = new Bar();
		hpBar->init(glm::vec2(0, 0), texProgram); //la funcio init() de Bar no fa us d'aquests valors
		hpBar->setType(0, texProgram);
		hpBar->setPlayer(player);
		expBar = new Bar();
		expBar->init(glm::vec2(0, 0), texProgram);
		expBar->setType(1, texProgram);
		expBar->setPlayer(player);
		
		keyUI = new Key();
		keyUI->init(glm::vec2(25 * map[0][0]->getTileSize(), 23 * map[0][0]->getTileSize()), texProgram);
		keyUI->setIdle();
		currentKeyState = false;

		setUpUISprites();
	}
	else if (currentState == END_GAME) {
		currentState = CREDITS;
	}
	else if (currentState == GAME_OVER || currentState == CREDITS) {
		currentState = MENU;
		Game::instance().playSound("gameMenu", true);
	}
}

//Code es un numero de 6 xifres ABXXYY, on A es el nivell, B es l'escena
//i XX i YY la posicio de la tile on es coloca el personatge
void Scene::changeScene(int code)
{
	map[currentState-1][currentScene-1]->setEntitiesIdle();
	int lvl = code / 100000;
	if (currentState != lvl)
		changeLevel(lvl);
	currentScene = (code / 10000) % 10;
	if(startLevelTime<=0)
		map[currentState-1][currentScene-1]->setEntitiesActive();
	float x = (code % 10000) / 100;
	float y = (code % 100);
	y += 0.5; //per que no quedi elevat
	player->setPosition(glm::vec2(x * map[currentState - 1][currentScene-1]->getTileSize(), y * map[currentState - 1][currentScene - 1]->getTileSize()));
	player->setTileMap(map[currentState - 1][currentScene - 1]);
	scenenNumber->changeNumber(currentScene);
}

void Scene::gameOver()
{
	Game::instance().playSound("gameOver", false);
	currentState = GAME_OVER;
}

void Scene::endGame()
{
	Game::instance().playSound("gameClear", false);
	currentState = END_GAME;
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

void Scene::createGame()
{
	for (int lvl = 0; lvl < 6; ++lvl) {
		for (int scene = 0; scene < 3; ++scene) {
			string path = std::string("levels/level0") + std::to_string(lvl + 1) + std::string("/scene0") + std::to_string(scene + 1) + std::string(".txt");
			map[lvl][scene] = TileMap::createTileMap(path, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
			map[lvl][scene]->setEntitiesIdle();
			map[lvl][scene]->setEntitiesPlayer(player);
			map[lvl][scene]->setLvl(lvl+1);

		}
	}
}

void Scene::changeLevel(int lvl)
{
	currentState = lvl;
	Game::instance().playSound(std::to_string(currentState), true);
	lvlNumber->changeNumber(currentState);
	startLevelTime = START_LEVEL_TIME;
	player->resetStartTime(startLevelTime);
	map[currentState-1][currentScene - 1]->setEntitiesIdle();
}

void Scene::setUpUISprites()
{
	//Power Ups
	powerUpsSpritesheet.loadFromFile("images/powerups.png", TEXTURE_PIXEL_FORMAT_RGBA);
	powerUpsUI.resize(NUM_POWER_UPS);
	for (int i = 0; i < powerUpsUI.size(); ++i) {
		Sprite *pwup = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.2f, 1.f), &powerUpsSpritesheet, &texProgram);
		pwup->setNumberAnimations(1);
		pwup->setAnimationSpeed(0, 8);
		pwup->addKeyframe(0, glm::vec2(0.2f * i, 0.f));
		pwup->changeAnimation(0);
		pwup->setPosition(glm::vec2((0.5f+i) * map[0][0]->getTileSize()*2, 22 * map[0][0]->getTileSize()));
		powerUpsUI[i] = pwup;
	}
	
	//Friends
	friendSpritesheet.loadFromFile("images/friendui.png", TEXTURE_PIXEL_FORMAT_RGBA);
	friends.resize(6);
	for (int i = 0; i < friends.size(); ++i) {
		friends[i] = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(1.f, 1.f), &friendSpritesheet, &texProgram);
		friends[i]->setNumberAnimations(0);
		if(i < 4){
			friends[i]->setPosition(glm::vec2((27+i) * map[0][0]->getTileSize(), 22 * map[0][0]->getTileSize()));
		}
		else
			friends[i]->setPosition(glm::vec2((27+i-4) * map[0][0]->getTileSize(), 23 * map[0][0]->getTileSize()));
	}

	//God mode text
	godmodeSpritesheet.loadFromFile("images/godmode.png", TEXTURE_PIXEL_FORMAT_RGBA);
	godmode = Sprite::createSprite(glm::ivec2(64, 8), glm::vec2(1.f, 1.f), &godmodeSpritesheet, &texProgram);
	godmode->setPosition(glm::vec2(map[0][0]->getTileSize(), 0));
}

void Scene::renderUI()
{
	prerenderedUI->render();
	lvlNumber->render();
	scenenNumber->render();
	hpBar->render();
	expBar->render();
	keyUI->render();
	for (int i = 0; i < NUM_POWER_UPS; ++i) {
		if (player->getPowerUps()[i])
			powerUpsUI[i]->render();
	}
	for (int i = 0; i < player->getNumFriends(); ++i)
		friends[i]->render();
	if (player->getGodMode())
		godmode->render();
}



