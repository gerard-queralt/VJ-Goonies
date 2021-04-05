#include "PandoraBubble.h"

#define DMG_CD 60
#define LO 0.5
#define HI 1.25

void PandoraBubble::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/bubble.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(0);

	horizontal = rand() % 3;
	vertical = rand() % 3;

	speed = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));

	dmgCD = DMG_CD;

	position = tileMapPos;
	startPosition = position;
	sprite->setPosition(position);
}

void PandoraBubble::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (horizontal == NEGATIVE) {
		position.x -= speed;
		if (position.x <= 0) {
			position.x = 0;
			horizontal = POSITIVE;
		}
	}
	else if(horizontal == POSITIVE){
		position.x += speed;
		if ((position.x + 16 /*size*/) >= (32 * map->getTileSize())) {
			position.x = (32 * map->getTileSize()) - 16 /*size*/;
			horizontal = NEGATIVE;
		}
	}
	if (vertical == NEGATIVE) {
		position.y -= speed;
		if (position.y <= 2 * map->getTileSize()) {
			position.y = 2 * map->getTileSize();
			vertical = POSITIVE;
		}
	}
	else if(vertical == POSITIVE){
		position.y += speed;
		if ((position.y + 16 /*size*/) >= (22 * map->getTileSize())) {
			position.y = (22 * map->getTileSize()) - 16 /*size*/;
			vertical = NEGATIVE;
		}
	}
	sprite->setPosition(position);
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
				if (horizontal == POSITIVE)
					horizontal = NEGATIVE;
				else if (horizontal == NEGATIVE)
					horizontal = POSITIVE;
				if (vertical == POSITIVE)
					vertical = NEGATIVE;
				else if (vertical == NEGATIVE)
					vertical = POSITIVE;
			}
		}
	}
}

void PandoraBubble::randomize()
{
	//1/9 de quedar-se quieta respecte un eix i 4/9 de moure's en cadascuna de les direccions
	horizontal = rand() % 9;
	vertical = rand() % 9;
	while (horizontal == STAY && vertical == STAY) {
		horizontal = rand() % 9;
		vertical = rand() % 9;
	}
	if (horizontal > STAY)
		horizontal = horizontal % 2 + 1;
	if (vertical > STAY)
		vertical = vertical % 2 + 1;

	speed = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}
