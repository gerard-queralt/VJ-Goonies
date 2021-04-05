#include "Waterfall.h"

#define FALL_TIME 3
#define STAY_TIME 70
#define CYCLE_TIME 100
#define DMG_CD 8

void Waterfall::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/waterfall.png", TEXTURE_PIXEL_FORMAT_RGBA);
	
	sprite = Sprite::createSprite(glm::ivec2(24, 8), glm::vec2(1.f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(1, 8);
	sprite->addKeyframe(1, glm::vec2(0.f, 0.75f));

	sprite->changeAnimation(0);

	middle.resize(0);

	top = Sprite::createSprite(glm::ivec2(24, 8), glm::vec2(1.f, 0.25f), &spritesheet, &shaderProgram);
	top->setNumberAnimations(1);

	top->setAnimationSpeed(0, 8);
	top->addKeyframe(0, glm::vec2(0.f, 0.f));

	top->changeAnimation(0);

	status = DEAD;
	started = false;
	fallTime = 0;
	secondFallTime = 0;
	stayTime = STAY_TIME;
	cycleTime = 0;
	dmgCD = DMG_CD;
	this->shaderProgram = shaderProgram;

	position = tileMapPos;
	position.x += 1 * map->getTileSize();
	startPosition = position;
	sprite->setPosition(position);
}

void Waterfall::render()
{
	if (status == ALIVE && started) {
		if (fallTime == FALL_TIME) {
			if (!map->waterfallCollision(position, glm::vec2(24, 8))) {
				position.y += map->getTileSize();
				fallTime = 0;
			}
			else {
				sprite->changeAnimation(1);
				--stayTime;
			}
		}
		else
			++fallTime;
		sprite->setPosition(position);
		sprite->render();
		if (stayTime <= 0) {
			if (!map->waterfallCollision(glm::vec2(position.x, position.y - middle.size() * map->getTileSize()), glm::vec2(24, 8))) {
				if (secondFallTime == FALL_TIME) {
					middle.erase(middle.begin());
					secondFallTime = 0;
				}
				else
					++secondFallTime;
			}
			else {
				status = DEAD;
				position = startPosition;
				return;
			}
		}
		for (int i = 0; i < middle.size(); ++i) {
			glm::vec2 newPos = position;
			newPos.y -= (1 + i) * map->getTileSize();
			middle[i]->setPosition(newPos);
		}
		if (middle.size() < (position.y - startPosition.y)/map->getTileSize() && stayTime > 0) {
			Sprite *tmp = Sprite::createSprite(glm::ivec2(24, 8), glm::vec2(1.f, 0.25f), &spritesheet, &shaderProgram);
			tmp->setNumberAnimations(1);

			tmp->setAnimationSpeed(0, 8);
			tmp->addKeyframe(0, glm::vec2(0.f, 0.25f));

			tmp->changeAnimation(0);

			tmp->setPosition(startPosition);
			middle.push_back(tmp);
		}
		for (int i = 0; i < middle.size(); ++i)
			middle[i]->render();
		if (stayTime <= 0) {
			top->setPosition(glm::vec2(position.x, position.y - middle.size() * map->getTileSize()));
			top->render();
		}
		if (dmgCD < DMG_CD)
			++dmgCD;
	}
	else if (status == DEAD && started) {
		if (cycleTime == CYCLE_TIME)
			setActive();
		else
			++cycleTime;
	}
}

void Waterfall::setIdle()
{
	status = DEAD;
	started = false;
}

void Waterfall::setActive()
{
	status = ALIVE;
	started = true;
	fallTime = 0;
	fallTime = 0;
	secondFallTime = 0;
	stayTime = STAY_TIME;
	cycleTime = 0;
	position = startPosition;
	sprite->setPosition(position);
	top->setPosition(startPosition);
	sprite->changeAnimation(0);
	middle.clear();
}

void Waterfall::interact()
{
	for (int y = position.y; y >= startPosition.y && y >= (position.y - middle.size() * map->getTileSize()) /*top position*/ && !player->getPowerUps()[3] /*blue watercoat*/; y -= map->getTileSize()) {
		if (inContactWithPlayer(glm::vec2(position.x+4, float(y)), glm::ivec2(16, 8))) { //ajustem a les hitboxes que esperem visualment
			if (dmgCD == DMG_CD) {
				player->hurt(2);
				dmgCD = 0;
			}
		}
	}
}
