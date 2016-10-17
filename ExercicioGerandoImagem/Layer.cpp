/* ___                                                        _            ___            __ _                       ___   _
  / _ \_ __ ___   ___ ___  ___ ___  __ _ _ __ ___   ___ _ __ | |_ ___     / _ \_ __ __ _ / _(_) ___ ___             / _ \ /_\
 / /_)/ '__/ _ \ / __/ _ \/ __/ __|/ _` | '_ ` _ \ / _ \ '_ \| __/ _ \   / /_\/ '__/ _` | |_| |/ __/ _ \   _____   / /_\///_\\
/ ___/| | | (_) | (_|  __/\__ \__ \ (_| | | | | | |  __/ | | | || (_) | / /_\\| | | (_| |  _| | (_| (_) | |_____| / /_\\/  _  \
\/    |_|  \___/ \___\___||___/___/\__,_|_| |_| |_|\___|_| |_|\__\___/  \____/|_|  \__,_|_| |_|\___\___/          \____/\_/ \_/
									Unisinos 2016 - Vinicius Pegorini Arnhold e Reni Steffenon
*/
#include "Layer.h"
#include <iostream>

#pragma warning( disable : 4244)


Layer::Layer()
{
}


void Layer::scroll(bool right)
{
	if (right) {
		if ((posX + rateX) > (background.getWidth()) - 1500) {
			posX = background.getWidth() - 1500;
		}
		else {
			posX += rateX;
		}
	}
	else {
		if ((posX - rateX) < 0) {
			posX = 0;
		}
		else {
			posX -= rateX;
		}
	}
}

void Layer::plot(Image * vpImage, char zBuffer[],int z)
{
	background.plotInto(vpImage, posX, posY, zBuffer, z);
	for (int i = 0; i < elements.size(); i++) {
		vpImage->plot(elements.at(i)->getFrame(), elements.at(i)->getPosX(), elements.at(i)->getPosY());
	}
}

void Layer::computeScrollRateX(int mainWidth)
{
	this->rateX = mainWidth;
}

void Layer::computeScrollRateY(int mainHeight)
{
	this->rateY = mainHeight;
}

void Layer::setBackground(Image image) {
	this->background = image;
}

void Layer::addGameObject(GameObject* go) {
	this->elements.push_back(go);
}

Image* Layer::getBackground() {
	return &background;
}

int Layer::getPosX()
{
	return posX;
}
