/* ___                                                        _            ___            __ _                       ___   _
  / _ \_ __ ___   ___ ___  ___ ___  __ _ _ __ ___   ___ _ __ | |_ ___     / _ \_ __ __ _ / _(_) ___ ___             / _ \ /_\
 / /_)/ '__/ _ \ / __/ _ \/ __/ __|/ _` | '_ ` _ \ / _ \ '_ \| __/ _ \   / /_\/ '__/ _` | |_| |/ __/ _ \   _____   / /_\///_\\
/ ___/| | | (_) | (_|  __/\__ \__ \ (_| | | | | | |  __/ | | | || (_) | / /_\\| | | (_| |  _| | (_| (_) | |_____| / /_\\/  _  \
\/    |_|  \___/ \___\___||___/___/\__,_|_| |_| |_|\___|_| |_|\__\___/  \____/|_|  \__,_|_| |_|\___\___/          \____/\_/ \_/
								Unisinos 2016 - Vinicius Pegorini Arnhold e Reni Steffenon
*/
#include "Image.h"


#pragma warning( disable : 4244)

Image::Image(int largura, int altura) {
	width = largura;
	height = altura;
	pixels = new unsigned int[width*height];
}

Image::Image() {
	width = 0;
	height = 0;
}


void Image::setWidth(int w) {
	width = w;
}

void Image::setHeight(int h) {
	height = h;
}


void Image::calcular() {
	pixels = new unsigned int[width*height];
}

void Image::setPixel(int rgb, int x, int y) {
	pixels[x + y*width] = rgb;
}

int Image::getPixel(int x, int y) {
	return pixels[x + y*width];
}

void Image::setPixel(int a, int r, int g, int b, int x, int y) {
	pixels[x + y*width] = (a << 24) | (r << 16) | (g << 8) | (b);
}
unsigned int* Image::getPixels() {
	return pixels;
}

int Image::getWidth() {
	return width;
}

int Image::getHeight() {
	return height;
}

Image Image::clone() {
	Image aux(getWidth(), getHeight());

	for (int x = 0; x < getWidth(); x++) {
		for (int y = 0; y < getHeight(); y++) {
			aux.setPixel(getPixel(x, y), x, y);
		}
	}
	return aux;

}

void Image::subImage(Image *src, int startx, int starty) {

	int memoryStartY = starty;
	for (int x = 0; x < src->getWidth(); x++) {
		for (int y = 0; y < src->getHeight(); y++) {
			src->setPixel(getPixel(startx, starty), x, y);
			starty++;
		}
		startx++;
		starty = memoryStartY;
	}
}

void Image::plotInto(Image* ref, int posicaoX, int posicaoY, char* zBuffer, char z)
{
	int xRef = 0;
	int yRef = 0;

	for (int x = posicaoX; x < ref->getWidth() + posicaoX && x < this->width; x++) {

		for (int y = posicaoY; y < ref->getHeight() + posicaoY && y < this->height; y++) {
			int alfa = (getPixel(x, y) >> 24) & 0xff;
			if (alfa != 0) {
				if (!alfa == 255) {
					if (z < zBuffer[xRef*yRef]) {
						ref->setPixel(calcularPixels(getPixel(x, y), ref->getPixel(xRef, yRef)), xRef, yRef);
					}
				}
				else {
					if (z < zBuffer[xRef*yRef]) {
						ref->setPixel(getPixel(x, y), xRef, yRef);
					}
					if (alfa != 255) {
						ref->setPixel(calcularPixels(getPixel(x, y), ref->getPixel(xRef, yRef)), xRef, yRef);
					}
					else {
						ref->setPixel(getPixel(x, y), xRef, yRef);
					}
				}
				yRef++;
			}
			yRef = 0;
			xRef++;
		}
	}
}

void Image::plot(Image* sobreposta, int posicaoX, int posicaoY) {

	int xSobreposta = 0;
	int ySobreposta = 0;

	for (int x = posicaoX; x < sobreposta->width + posicaoX; x++) {
		for (int y = posicaoY; y < posicaoY + sobreposta->height; y++) {
			int pixelSobreposta = sobreposta->getPixel(xSobreposta, ySobreposta);
			int alfa = (pixelSobreposta >> 24) & 0xff;
			if (alfa == 0) {
			}
			else {
				if (alfa == 255) {
					setPixel(pixelSobreposta, x, y);
				}
				else {
					setPixel(calcularPixels(pixelSobreposta, getPixel(x, y)), x, y);
				}
			}
			ySobreposta++;

		}
		ySobreposta = 0;
		xSobreposta++;
	}

}

int Image::calcularPixels(int sobreposta, int fundo) {
	int aSobreposta = (sobreposta >> 24) & 0xff;
	int rSobreposta = (sobreposta >> 16) & 0xff;
	int gSobreposta = (sobreposta >> 8) & 0xff;
	int bSobreposta = sobreposta & 0xff;

	int aFundo = (fundo >> 24) & 0xff;
	int rFundo = (fundo >> 16) & 0xff;
	int gFundo = (fundo >> 8) & 0xff;
	int bFundo = fundo & 0xff;

	double alfa = aSobreposta / 255;
	int r = 0;
	int g = 0;
	int b = 0;

	r = rFundo*(1 - alfa) + rSobreposta*alfa;
	g = gFundo*(1 - alfa) + gSobreposta*alfa;
	b = bFundo*(1 - alfa) + bSobreposta*alfa;

	int resultado = (255 << 24) | (r << 16) | (g << 8) | (b);

	return resultado;
}