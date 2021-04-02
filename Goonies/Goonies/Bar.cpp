#include <cmath>
#include "Bar.h"

enum Type {
	HEALTH, EXP
};

void Bar::init(const glm::vec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	bars.resize(10);
	for (int i = 0; i < bars.size(); ++i) {
		bars[i] = new SubBar();
	}
}

void Bar::render()
{
	for (SubBar *b : bars) {
		b->render();
	}
}

void Bar::update(int deltaTime)
{
	int var;
	if (type == HEALTH) {
		var = player->getHealth();
	}
	else {
		var = player->getExp();
	}
	int n = ceil(float(var) / 8.f) - 1;
	if (var % 8 == 0) {
		if (n < 0) {
			n = 0;
			bars[n]->setAnimation(0);
		}
		else
			bars[n]->setAnimation(8);
	}
	else
		bars[n]->setAnimation(var % 8);
	for (int i = n + 1; i < bars.size(); ++i) {
		bars[i]->setAnimation(0);
	}
	for (int i = 0; i < n; ++i) {
		bars[i]->setAnimation(8);
	}
}

void Bar::setType(int type, ShaderProgram & shaderProgram)
{
	this->type = type;
	for (int i = 0; i < bars.size(); ++i) {
		bars[i]->setType(type);
		bars[i]->init(glm::vec2((15+i) * 8.f, (type) * 8.f), shaderProgram);
	}
}
