#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 16
#define FALL_STEP 1
#define SPEED 0.5
#define SHOES_SPEED_UP 0.5
#define CLIMB_SPEED 1


enum PlayerAnims
{
	STAND_LEFT=0, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, PUNCH_LEFT, PUNCH_RIGHT, IDLE_CLIMBING, CLIMBING_ANIM, NUM_ANIM
};

enum PowerUps
{
	HELMET = 0, HYPER_SHOES, WATERCOAT_GREY, WATERCOAT_BLUE, SPELLBOOK, NUM_POWER_UPS
};

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	health = 80;
	level = 1;
	exp = 0;
	std::vector<bool> tmp(NUM_POWER_UPS, false);
	powerUps.resize(NUM_POWER_UPS);
	powerUps = tmp;
	hasKey = false;
	numFriends = 0;

	status = GROUNDED;
	startTime = 0;
	punchingTime = 8;
	spaceKeyStatus = HOLD;
	spritesheet.loadFromFile("images/playersprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIM);
	
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.25f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.25f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.75f, 0.f));

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.75f, 0.25f));

		sprite->setAnimationSpeed(PUNCH_RIGHT, 8);
		sprite->addKeyframe(PUNCH_RIGHT, glm::vec2(0.5f, 0.5f));

		sprite->setAnimationSpeed(PUNCH_LEFT, 8);
		sprite->addKeyframe(PUNCH_LEFT, glm::vec2(0.75f, 0.5f));

		sprite->setAnimationSpeed(IDLE_CLIMBING, 8);
		sprite->addKeyframe(IDLE_CLIMBING, glm::vec2(0.f, 0.5f));

		sprite->setAnimationSpeed(CLIMBING_ANIM, 8);
		sprite->addKeyframe(CLIMBING_ANIM, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(CLIMBING_ANIM, glm::vec2(0.25f, 0.5f));

	sprite->changeAnimation(STAND_RIGHT);
	lookingLeft = false;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x) + posPlayer.x, float(tileMapDispl.y) + posPlayer.y));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (startTime == 0) {
		if (sprite->animation() == MOVE_LEFT || sprite->animation() == MOVE_RIGHT)
			keepMovingInAir = true;
		if (status == PUNCHING) {
			if (punchingTime == 0) {
				status = GROUNDED;
				if (sprite->animation() == PUNCH_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == PUNCH_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
			}
			else
				--punchingTime;
		}
		bool key = Game::instance().getKey(32); //space
		if (spaceKeyStatus == PRESSED) {
			if (key)
				spaceKeyStatus = HOLD;
			else
				spaceKeyStatus = RELEASED;
		}
		else if (spaceKeyStatus == HOLD) {
			if (!key)
				spaceKeyStatus = RELEASED;
		}
		else if (spaceKeyStatus == RELEASED) {
			if (key)
				spaceKeyStatus = PRESSED;
		}
		if (spaceKeyStatus == PRESSED && status == GROUNDED && sprite->animation() != PUNCH_RIGHT && sprite->animation() != PUNCH_LEFT) {
			status = PUNCHING;
			punchingTime = 10;
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT)
				sprite->changeAnimation(PUNCH_LEFT);
			else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT)
				sprite->changeAnimation(PUNCH_RIGHT);
		}
		else if ((Game::instance().getSpecialKey(GLUT_KEY_LEFT) && status == GROUNDED) || (sprite->animation() == JUMP_LEFT && keepMovingInAir))
		{
			if (sprite->animation() != MOVE_LEFT && status == GROUNDED)
				sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= SPEED;
			if (powerUps[HYPER_SHOES])
				posPlayer.x -= SHOES_SPEED_UP;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(16, 10)))
			{
				posPlayer.x += SPEED;
				if (powerUps[HYPER_SHOES])
					posPlayer.x += SHOES_SPEED_UP;
				keepMovingInAir = false;
				sprite->changeAnimation(STAND_LEFT);
			}
			lookingLeft = true;
		}
		else if ((Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && status == GROUNDED) || (sprite->animation() == JUMP_RIGHT && keepMovingInAir))
		{
			if (sprite->animation() != MOVE_RIGHT && status == GROUNDED)
				sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += SPEED;
			if (powerUps[HYPER_SHOES])
				posPlayer.x += SHOES_SPEED_UP;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(16, 10)))
			{
				posPlayer.x -= SPEED;
				if (powerUps[HYPER_SHOES])
					posPlayer.x -= SHOES_SPEED_UP;
				keepMovingInAir = false;
				sprite->changeAnimation(STAND_RIGHT);
			}
			lookingLeft = false;
		}
		else if(status == GROUNDED)
		{
			if (sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}

		if (status == JUMPING)
		{
			if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT)
				sprite->changeAnimation(JUMP_LEFT);
			else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT)
				sprite->changeAnimation(JUMP_RIGHT);
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				status = FALLING;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90)
					if (map->collisionMoveDown(posPlayer, glm::ivec2(16, 10), &posPlayer.y))
						status = GROUNDED;
					else
						status = FALLING;
			}
		}
		else if (status == CLIMBING) {
			if (Game::instance().getSpecialKey(GLUT_KEY_UP)) 
			{
				if(sprite->animation()!=CLIMBING_ANIM)
					sprite->changeAnimation(CLIMBING_ANIM);
				posPlayer.y -= CLIMB_SPEED;
				if (map->stopClimbing(posPlayer, glm::ivec2(10, 16), true))
				{
					posPlayer.y -= 12; //segurament se pot fer millor en una deteccio de col·lisions diferent
					status = GROUNDED;
					if (lookingLeft)
						sprite->changeAnimation(STAND_LEFT);
					else
						sprite->changeAnimation(STAND_RIGHT);
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) 
			{
				if (sprite->animation() != CLIMBING_ANIM)
					sprite->changeAnimation(CLIMBING_ANIM);
				posPlayer.y += CLIMB_SPEED;
				if (map->stopClimbing(posPlayer, glm::ivec2(10, 16), false))
				{
					status = GROUNDED;
					if(lookingLeft)
						sprite->changeAnimation(STAND_LEFT);
					else
						sprite->changeAnimation(STAND_RIGHT);
				}
			}
			else
				sprite->changeAnimation(IDLE_CLIMBING);
		}
		else
		{
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(12, 10), &posPlayer.y))
			{
				if (sprite->animation() == JUMP_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == JUMP_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
				if (status != PUNCHING)
					status = GROUNDED;
				keepMovingInAir = false;
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
				{
					if (status == GROUNDED && map->climbUp(posPlayer, glm::ivec2(16, 16), &posPlayer.x, lookingLeft)) {
						sprite->changeAnimation(IDLE_CLIMBING);
						status = CLIMBING;
					}
					else {
						status = JUMPING;
						jumpAngle = 0;
						startY = posPlayer.y;
					}
				}
				else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
				{
					if (status == GROUNDED && map->climbDown(posPlayer, glm::ivec2(16, 16), &posPlayer.x, lookingLeft)) {
						sprite->changeAnimation(IDLE_CLIMBING);
						status = CLIMBING;
					}
				}
			}
		}
	}
	else
		--startTime;
	
	int state;
	switch (sprite->animation())
	{
	case STAND_RIGHT:
	case MOVE_RIGHT:
	case JUMP_RIGHT:
	case PUNCH_RIGHT:
		state = 1;
		break;
	case STAND_LEFT:
	case MOVE_LEFT:
	case JUMP_LEFT:
	case PUNCH_LEFT:
		state = 2;
		break;
	default:
		state = 0;
		break;
	}
	map->detectChangeScene(posPlayer, glm::ivec2(16, 16), state);
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

int Player::getStatus()
{
	return status;
}

glm::vec2 Player::getPosition()
{
	return posPlayer;
}

void Player::heal(int heal)
{
	if(health+heal <= 80)
		health += heal;
}

void Player::hurt(int dmg)
{
	//falta que el personatge se pose blanc
	if (health - dmg >= 0)
		health -= dmg;
	else
		health = 0; //aqui hauria d'acabar la partida
}

void Player::gainXP(int exp)
{
	this->exp += exp;
	if (this->exp >= 80) {
		++level;
		this->exp -= 80;
	}
}

int Player::getHealth()
{
	return health;
}

int Player::getExp()
{
	return exp;
}

bool Player::getHasKey()
{
	return hasKey;
}

bool Player::getKey()
{
	if (!hasKey) {
		hasKey = true;
		return true;
	}
	return false;
}

bool Player::useKey()
{
	if (hasKey) {
		hasKey = false;
		return true;
	}
	return false;
}

void Player::givePowerUp(int pwup)
{
	powerUps[pwup] = true;
}

std::vector<bool> Player::getPowerUps()
{
	return powerUps;
}

void Player::rescueFriend()
{
	if (numFriends < 5)
		++numFriends;
}

int Player::getNumFriends()
{
	return numFriends;
}

void Player::resetStartTime(int time)
{
	startTime = time;
	sprite->changeAnimation(STAND_RIGHT);
}




