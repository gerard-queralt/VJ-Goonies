#include "PandoraBubble.h"

#define DMG_CD 60
#define LO 0.5
#define HI 1.25

void PandoraBubble::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/bubble.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	movingLeft = rand() % 2;
	movingUp = rand() % 2;

	speed = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));

	dmgCD = DMG_CD;

	position = tileMapPos;
	startPosition = position;
	sprite->setPosition(position);
}

void PandoraBubble::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (movingLeft) {
		position.x -= speed;
		if (position.x <= 0) {
			position.x = 0;
			movingLeft = false;
		}
	}
	else {
		position.x += speed;
		if ((position.x + 16 /*size*/) >= (32 * map->getTileSize())) {
			position.x = (32 * map->getTileSize()) - 16 /*size*/;
			movingLeft = true;
		}
	}
	if (movingUp) {
		position.y -= speed;
		if (position.y <= 2 * map->getTileSize()) {
			position.y = 2 * map->getTileSize();
			movingUp = false;
		}
	}
	else {
		position.y += speed;
		if ((position.y + 16 /*size*/) >= (22 * map->getTileSize())) {
			position.y = (22 * map->getTileSize()) - 16 /*size*/;
			movingUp = true;
		}
	}
	sprite->setPosition(position);
}

void PandoraBubble::setIdle()
{
}

void PandoraBubble::setActive()
{
}

void PandoraBubble::interact()
{
	if (status != DEAD) {
		if (dmgCD < DMG_CD)
			++dmgCD;
		if (inContactWithPlayer(position, glm::ivec2(8, 8))) {
			if (dmgCD == DMG_CD) {
				player->hurt(5);
				dmgCD = 0;
				movingLeft = !movingLeft;
				movingUp = !movingUp;
			}
		}
	}
}

void PandoraBubble::randomize()
{
	movingLeft = rand() % 2;
	movingUp = rand() % 2;

	speed = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}
