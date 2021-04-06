#include <iostream>
#include <fstream>
#include <sstream>
#include "TileMap.h"
#include "Game.h"
#include "Entity.h"
#include "Skull.h"
#include "Stalactite.h"
#include "WaterDrop.h"
#include "Waterfall.h"
#include "StartDoor.h"
#include "LockedDoor.h"
#include "DoubleLockedDoor.h"
#include "Key.h"
#include "Pouch.h"
#include "EndDoor.h"
#include "Pandora.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::update(int deltaTime)
{
	for (Entity* e : entities) {
		e->interact();
		e->update(deltaTime);
	}
}

void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
	for (Entity* e : entities)
		e->render();
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

void TileMap::setEntitiesIdle()
{
	for (Entity* e : entities)
		e->setIdle();
}

void TileMap::setEntitiesActive()
{
	for (Entity* e : entities)
		e->setActive();
}

void TileMap::setEntitiesPlayer(Player * player)
{
	for (Entity* e : entities)
		e->setPlayer(player);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			int tile;
			fin >> tile;
			map[j*mapSize.x+i] = tile;
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile > 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)% tilesheetSize.x) / tilesheetSize.x, float((tile-1)/ tilesheetSize.y) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
			else if (tile < 0) {
				switch (tile)
				{
				case -1: //calavera recta
				{
					Skull *skull = new Skull();
					skull->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					skull->setTileMap(this);
					skull->setBehaviour(0);
					entities.push_back(skull);
					break;
				}
				case -2: //calaver saltarina
				{
					Skull *skull = new Skull();
					skull->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					skull->setTileMap(this);
					skull->setBehaviour(1);
					entities.push_back(skull);
					break;
					break;
				}
				case -3: //estalactita roja
				{
					Stalactite *stal = new Stalactite();
					stal->setColor(0);
					stal->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					stal->setTileMap(this);
					entities.push_back(stal);
					break;
				}
				case -17: //estalactita blava
				{
					Stalactite *stal = new Stalactite();
					stal->setColor(1);
					stal->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					stal->setTileMap(this);
					entities.push_back(stal);
					break;
				}
				case -20: //estalactita verda
				{
					Stalactite *stal = new Stalactite();
					stal->setColor(2);
					stal->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					stal->setTileMap(this);
					entities.push_back(stal);
					break;
				}
				case -4: //gota
				{
					WaterDrop *wd = new WaterDrop();
					wd->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					wd->setTileMap(this);
					entities.push_back(wd);
					break;
				}
				case -5: //cascada
				{
					Waterfall *wf = new Waterfall();
					wf->setTileMap(this);
					wf->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					entities.push_back(wf);
					break;
				}
				case -6: //porta inici
				{
					StartDoor *startDoor = new StartDoor();
					startDoor->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					startDoor->setTileMap(this);
					entities.push_back(startDoor);
					break;
				}
				case -7: //porta roja amb un candau i pocio
				{
					LockedDoor *ld = new LockedDoor();
					ld->setContent(0);
					ld->setColor(0);
					ld->setTileMap(this);
					ld->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					entities.push_back(ld);
					break;
				}
				case -8: //porta roja amb un candau i amic
				{
					LockedDoor *ld = new LockedDoor();
					ld->setContent(1);
					ld->setColor(0);
					ld->setTileMap(this);
					ld->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					entities.push_back(ld);
					break;
				}
				case -18: //porta blava amb un candau i pocio
				{
					LockedDoor *ld = new LockedDoor();
					ld->setContent(0);
					ld->setColor(1);
					ld->setTileMap(this);
					ld->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					entities.push_back(ld);
					break;
				}
				case -19: //porta blava amb un candau i amic
				{
					LockedDoor *ld = new LockedDoor();
					ld->setContent(1);
					ld->setColor(1);
					ld->setTileMap(this);
					ld->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					entities.push_back(ld);
					break;
				}
				case -21: //porta verda amb un candau i pocio
				{
					LockedDoor *ld = new LockedDoor();
					ld->setContent(0);
					ld->setColor(2);
					ld->setTileMap(this);
					ld->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					entities.push_back(ld);
					break;
				}
				case -22: //porta verda amb un candau i amic
				{
					LockedDoor *ld = new LockedDoor();
					ld->setContent(1);
					ld->setColor(2);
					ld->setTileMap(this);
					ld->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					entities.push_back(ld);
					break;
				}
				case -9: //porta roja amb dos candaus
				{
					DoubleLockedDoor *dld = new DoubleLockedDoor();
					dld->setTileMap(this);
					dld->setColor(0);
					dld->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					entities.push_back(dld);
					break;
				}
				case -23: //porta verda amb dos candaus
				{
					DoubleLockedDoor *dld = new DoubleLockedDoor();
					dld->setTileMap(this);
					dld->setColor(1);
					dld->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					entities.push_back(dld);
					break;
				}
				case -10: //clau
				{
					Key *key = new Key();
					key->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					key->setTileMap(this);
					entities.push_back(key);
					break;
				}
				//sac
				case -11:
				case -12:
				case -13:
				case -14:
				case -15:
				case -16:
				{
					Pouch *p = new Pouch();
					p->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					p->setTileMap(this);
					int content = ((tile * -1) % 10) - 1;
					p->setContent(content);
					entities.push_back(p);
					break;
				}
				case -24: //porta del final
				{
					EndDoor *endd = new EndDoor();
					endd->setTileMap(this);
					endd->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					entities.push_back(endd);
					break;
				}
				case -30: //Pandora
				{
					Pandora *pandora = new Pandora();
					pandora->setTileMap(this);
					pandora->init(glm::vec2(float(i)*tileSize, float(j)*tileSize), program);
					entities.push_back(pandora);
					break;
				}
				default:
					break;
				}
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

bool TileMap::notWalkable(int tile) const //solid
{
	return (tile == 1 || tile == 2 || tile == 3 || tile == 4 || tile == 111 || tile == 112 || (80<tile && tile<87) ||
			(12<tile && tile<16) || tile == 68 || tile == 158 || tile == 176);
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::vec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(notWalkable(map[y*mapSize.x+x]))
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::vec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(notWalkable(map[y*mapSize.x+x]))
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::vec2 &pos, const glm::ivec2 &size, float *posY) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(notWalkable(map[y*mapSize.x+x]))
		{
			if(*posY - tileSize * y + size.y <= 1)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveDownEntities(const glm::vec2 &pos, const glm::ivec2 &size) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (notWalkable(map[y*mapSize.x + x]))
		{
			return true;
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(const glm::vec2 &pos, const glm::ivec2 &size) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (notWalkable(map[y*mapSize.x + x]))
		{
			return true;
		}
	}

	return false;
}

bool TileMap::climbUp(const glm::vec2 & pos, const glm::ivec2 & size, float * posX, bool lookingLeft) const
{
	int x0, x1, y;
	
	if (lookingLeft) {
		x0 = (pos.x / tileSize) - 1;
		x1 = (pos.x + (size.x)) / tileSize;
	}
	else {
		x0 = (pos.x) / tileSize;
		x1 = (pos.x + (size.x - 1)) / tileSize;
	}
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] == 2 /*tile liana al terra*/)
		{
			if (*posX != (y*mapSize.x + x) % mapSize.x)
			{
				*posX = (y*mapSize.x + x) % mapSize.x * tileSize - 3;
				return true;
			}
		}
	}
	return false;
}

bool TileMap::climbDown(const glm::vec2 &pos, const glm::ivec2 &size, float *posX, bool lookingLeft) const
{
	int x0, x1, y;

	if (lookingLeft) {
		x0 = (pos.x / tileSize) - 1;
		x1 = (pos.x + (size.x)) / tileSize;
	}
	else {
		x0 = (pos.x) / tileSize;
		x1 = (pos.x + (size.x - 1)) / tileSize;
	}
	y = (pos.y + size.y - 1) / tileSize;
	y += 1; //mirem la tile de sota
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] == 100 /*tile liana al sostre*/)
		{
			if (*posX != (y*mapSize.x + x) % mapSize.x)
			{
				*posX = (y*mapSize.x + x) % mapSize.x * tileSize - 3;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::stopClimbing(const glm::vec2 & pos, const glm::ivec2 & size, bool up) const
{
	int x, y;
	x = (pos.x + size.x) / tileSize;
	if (up) {
		y = (pos.y - 1) / tileSize;
		++y; //la següent tile
		if (map[y*mapSize.x + x] != 101 /*tile liana*/ &&
			map[y*mapSize.x + x] != 100 /*tile liana al sostre*/)
			return true;
		return false;
	}
	else {
		y = (pos.y + size.y) / tileSize;
		if (map[y*mapSize.x + x] != 101 /*tile liana*/ &&
			map[y*mapSize.x + x] != 2 /*tile liana al terra*/)
			return true;
		return false;
	}
}

//0 baix, 1 dreta, 2 esquerra, 3 dalt
void TileMap::detectChangeScene(glm::vec2 & pos, const glm::ivec2 & size, int state) const
{
	if (state == 0) {
		//detectem tile de transcicio a baix
		int x0, x1, y;

		x0 = pos.x / tileSize;
		x1 = (pos.x + size.x - 1) / tileSize;
		y = (pos.y + size.y - 1) / tileSize;
		for (int x = x0; x <= x1; x++)
		{
			if (map[y*mapSize.x + x] < -100000 && map[y*mapSize.x + x] / 100000 == lvl * -1) {
				Game::instance().changeScene(map[y*mapSize.x + x] * -1);
				pos.x += (tileSize/2) + 1; //corregim la posicio a la liana
				return;
			}
		}
	}

	else if (state == 1) {
		//detectem tile de transcicio a la dreta
		int x, y0, y1;

		x = (pos.x + size.x - 1) / tileSize;
		y0 = pos.y / tileSize;
		y1 = (pos.y + size.y - 1) / tileSize;
		for (int y = y0; y <= y1; y++)
		{
			if (map[y*mapSize.x + x] < -100000 && map[y*mapSize.x + x] / 100000 == lvl * -1) {
				Game::instance().changeScene(map[y*mapSize.x + x] * -1);
				return;
			}
		}
	}

	else if (state == 2) {
		//detectem tile de transcicio a l'esquerra
		int x, y0, y1;

		x = (pos.x) / tileSize;
		y0 = pos.y / tileSize;
		y1 = (pos.y + size.y - 1) / tileSize;
		for (int y = y0; y <= y1; y++)
		{
			if (map[y*mapSize.x + x] < -100000 && map[y*mapSize.x + x] / 100000 == lvl * -1) {
				Game::instance().changeScene(map[y*mapSize.x + x] * -1);
				return;
			}
		}
	}
	else if (state == 3) {
		//detectem la transcicio a dalt
		int x0, x1, y;

		x0 = pos.x / tileSize;
		x1 = (pos.x + size.x - 1) / tileSize;
		y = (pos.y - 1) / tileSize;
		for (int x = x0; x <= x1; x++)
		{
			if (map[y*mapSize.x + x] < -100000 && map[y*mapSize.x + x] / 100000 == lvl * -1) {
				Game::instance().changeScene(map[y*mapSize.x + x] * -1);
				pos.x += (tileSize / 2) + 1; //corregim la posicio a la liana
				return;
			}
		}
	}
}

void TileMap::detectChangeLevel(glm::vec2 & pos, const glm::ivec2 & size) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	++y; //mirem la de baix
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] < -100000) {
			Game::instance().changeScene(map[y*mapSize.x + x] * -1);
			pos.x += (tileSize / 2); //corregim la posicio a la porta
			return;
		}
	}
}

bool TileMap::waterfallCollision(const glm::vec2 & pos, const glm::ivec2 & size) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] == 31 || map[y*mapSize.x + x] == 32)
		{
			return true;
		}
	}

	return false;
}

void TileMap::setLvl(int lvl)
{
	this->lvl = lvl;
}

glm::vec2 TileMap::getMapSize()
{
	return mapSize;
}






























