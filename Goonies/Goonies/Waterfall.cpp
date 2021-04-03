#include "Waterfall.h"

#define FALL_TIME 10
#define STAY_TIME 100
#define ANIM_TIME 5

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
	fallTime = 0;
	secondFallTime = 0;
	stayTime = STAY_TIME;
	stayAnimationTime = 0;
	this->shaderProgram = shaderProgram;

	position = tileMapPos;
	position.x += 1 * map->getTileSize();
	startPosition = position;
	sprite->setPosition(position);
}

void Waterfall::render()
{
	if (status == ALIVE) {
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
			else
				status = DEAD;
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
		//intent d'aconseguir l'efecte de cascada
		/*
		if (stayTime < STAY_TIME && stayTime > 0) {
			if (stayAnimationTime == 0) {
				middle.erase(middle.begin());
				stayAnimationTime = STAY_TIME;
			}
			else
				--stayAnimationTime;
		}
		*/
		for (int i = 0; i < middle.size(); ++i)
			middle[i]->render();
		if (stayTime <= 0) {
			top->setPosition(glm::vec2(position.x, position.y - middle.size() * map->getTileSize()));
			top->render();
		}
	}
}

void Waterfall::setIdle()
{
	status = DEAD;
}

void Waterfall::setActive()
{
	status = ALIVE;
	fallTime = 0;
	fallTime = 0;
	secondFallTime = 0;
	stayTime = STAY_TIME;
	stayAnimationTime = 0;
	position = startPosition;
	sprite->changeAnimation(0);
	middle.clear();
}

void Waterfall::interact()
{
	for (int y = position.y; y >= startPosition.y; y -= map->getTileSize()) {
		if (inContactWithPlayer(glm::vec2(position.x, float(y)), glm::ivec2(24, 8))) {
			player->hurt(5); //random number
			break;
		}
	}
}
