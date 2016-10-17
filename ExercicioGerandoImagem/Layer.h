#include "Image.h"
#include "GameObject.h"
#include <vector>
#pragma once

using namespace std;
class Layer
{
	float rateX = 0;
	float rateY = 0;
	float posX = 0;
	float posY = 0;
	Image background;
	vector<GameObject*> elements;

public:
	Layer();
	void scroll(bool right);
	void plot(Image* vpImage, char zBuffer[],int z);
	void computeScrollRateX(int mainWidth);
	void computeScrollRateY(int mainHeight);
	void setBackground(Image image);
	void addGameObject(GameObject* ob);
	Image* getBackground();
	
	int getPosX();
};

