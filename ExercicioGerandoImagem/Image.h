#pragma once
class Image
{
	unsigned int *pixels;
	int width=0;
	int height=0;
public:
	Image(int w, int h);
	Image();

	//Core
	void plot(Image* sobreposta, int posicaoX, int posicaoY);
	void subImage(Image* dest, int startx, int starty);
	void plotInto(Image* sobreposta, int posicaoX, int posicaoY, char *zBuffer, char z);

	//Helpers
	void calcular();
	int calcularPixels(int sobreposta, int fundo);
	Image clone();

	//Getters & Setters
	int getPixel(int x, int y);
	void setPixel(int rgb, int x, int y);
	void setPixel(int a, int r, int g, int b, int x, int y);
	unsigned int* getPixels();
	int getWidth();
	void setWidth(int w);
	int getHeight();
	void setHeight(int h);
};
