#include <iostream>
#include <Windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#pragma comment(lib, "winmm.lib")

void Game::init()
{
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::changeScene(int code)
{
	scene.changeScene(code);
}

void Game::gameOver()
{
	scene.gameOver();
}

void Game::endGame()
{
	scene.endGame();
}

void Game::playSound(string filename, bool loop)
{
	string path = "sounds/" + filename + ".wav";
	if (loop) {
		PlaySound(NULL, 0, 0);
		PlaySound(s2ws(path).c_str(), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}
	else {
		PlaySound(s2ws(path).c_str(), NULL, SND_ASYNC | SND_FILENAME);
	}
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
	scene.startGame();
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

wstring Game::s2ws(const string & s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}





