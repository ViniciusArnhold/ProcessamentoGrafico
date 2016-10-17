/* ___                                                        _            ___            __ _                       ___   _
  / _ \_ __ ___   ___ ___  ___ ___  __ _ _ __ ___   ___ _ __ | |_ ___     / _ \_ __ __ _ / _(_) ___ ___             / _ \ /_\
 / /_)/ '__/ _ \ / __/ _ \/ __/ __|/ _` | '_ ` _ \ / _ \ '_ \| __/ _ \   / /_\/ '__/ _` | |_| |/ __/ _ \   _____   / /_\///_\\
/ ___/| | | (_) | (_|  __/\__ \__ \ (_| | | | | | |  __/ | | | || (_) | / /_\\| | | (_| |  _| | (_| (_) | |_____| / /_\\/  _  \
\/    |_|  \___/ \___\___||___/___/\__,_|_| |_| |_|\___|_| |_|\__\___/  \____/|_|  \__,_|_| |_|\___\___/          \____/\_/ \_/
								Unisinos 2016 - Vinicius Pegorini Arnhold e Reni Steffenon
*/

#pragma once

#include <Windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include  <stdio.h>
#include <stdlib.h>
#include "Image.h"
#include "PTMReader.h"
#include <iostream>
#include "Layer.h"
#include "Animation.h"
#include "Timer.h"


#pragma warning( disable : 4244)//Conversao sempre estara no range

using namespace std;

//Tamanhos
int xPA = 228;
int yPA = 248;
int xPP = 216;
int yPP = 240;
int xCobra = 300;
int yCobra = 260;

vector<Layer> layers;
Image scene, backup;
char *zBuffer, *zBuffer2;

Animation animPerAndando = Animation();
Animation animPerPulando = Animation();
Animation animCobra = Animation();
Animation animCaixa = Animation();
Animation animSpike = Animation();

GameObject objPerPulando = GameObject();
GameObject objPerAndando = GameObject();
GameObject objCobra = GameObject();
GameObject objCaixa = GameObject();
GameObject objSpike = GameObject();
Timer timer = Timer();

PTMReader leitor = PTMReader();
Layer layer = Layer();

bool playing = true;

bool cobraEstaEscondia();
void validaColisao();
void finalize(bool ganhou);
void dispose();

void updateScene(int value) {
	scene = Image(1500, 600);
	int z = 5;
	for (int i = 0; i < layers.size(); i++) {
		layers.at(i).plot(&scene, zBuffer,z);
		z--;
	}
	for (int i = 0; i < 1500 * 600; i++) {
	}
		zBuffer[i] = zBuffer2[i];

}

void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glDrawPixels(1500, 600, GL_BGRA_EXT, GL_UNSIGNED_BYTE,
		scene.getPixels());
	glFlush();
}

void update(int value) {
	timer.start();
	objCobra.incCurrentFrame();
	objPerAndando.incCurrentFrame();
	updateScene(1);
	glutPostRedisplay();
	timer.finish();
	int waitingTime = timer.calcWaitingTime(12, timer.getElapsedTimeMs());
	if (playing) {
		glutTimerFunc(100, update, 5);
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'd':
		for (int i = 0; i < layers.size(); i++)
		{
			layers.at(i).scroll(true);
			validaColisao();
		}
		objCobra.setPosX(objCobra.getPosX() - 50);
		break;
	case 'a':
		for (int i = 0; i < layers.size(); i++)
		{
			layers.at(i).scroll(false);
			validaColisao();
		}
		objCobra.setPosX(objCobra.getPosX() + 50);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void validaColisao() {
	int posPer = objPerAndando.getPosX();
	int posCobra = objCobra.getPosX();
	cout << "---------------" << endl;
	cout << "posPer" << posPer << endl;
	cout << "posCobra" << posCobra << endl;
	cout << "widthCobra" << xCobra << endl;
	cout << "Escondida " << (cobraEstaEscondia()) << endl;
	cout << "---------------" << endl;
	if (layers.at(4).getPosX() > 5000) {
		finalize(true);
	}
	else if (!cobraEstaEscondia()) {
		if (posPer>posCobra && posCobra+xCobra> posPer) {
			finalize(false);
		}
	}
}

bool cobraEstaEscondia() {//Algoritimo de colisao -> O(11)
	int f = objCobra.getCurFrameNum();
	return f == 13 || f == 14 || f == 15 || f == 16 || f == 17 || f == 18 || f == 19 || f == 20 || f == 21 || f == 22 || f == 23;
}

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	/*
		1) carregar imagens das camadas e   2) inicia a layer
	*/
	leitor.ler("..\\Imagens\\zMinus3.ptm");
	layer.setBackground(leitor.getImage());
	layers.push_back(layer);
	leitor.ler("..\\Imagens\\zMinus2.ptm");
	layer = Layer();
	layer.setBackground(leitor.getImage());
	layers.push_back(layer);
	leitor.ler("..\\Imagens\\zMinus1.ptm");
	layer = Layer();
	layer.setBackground(leitor.getImage());
	layers.push_back(layer);
	leitor.ler("..\\Imagens\\zMinus0.ptm");
	layer = Layer();
	layer.setBackground(leitor.getImage());
	layers.push_back(layer);
	leitor.ler("..\\Imagens\\zPlus1.ptm");
	layer = Layer();
	layer.setBackground(leitor.getImage());
	layers.push_back(layer);

	/*
		3) Carregar anima��es objetos do jogo
	*/
	leitor.ler("..\\Imagens\\Andando.ptm");
	Image backup = Image(xPA, yPA);
	int size = leitor.getImage().getWidth();
	for (int i = 0; i < size / xPA; i++) {
		leitor.getImage().subImage(&backup, i*xPA, 0);
		animPerAndando.addFrame(backup);
	}

	leitor.ler("..\\Imagens\\Cobras.ptm");
	backup = Image(xCobra, yCobra);
	size = leitor.getImage().getWidth();
	for (int i = 0; i < size / xCobra; i++) {
		leitor.getImage().subImage(&backup, i*xCobra, 0);
		animCobra.addFrame(backup);
	}
	objPerAndando.setSprite(&animPerAndando);
	objPerAndando.setPosX(100);
	objPerAndando.setPoxY(200);
	objCobra.setSprite(&animCobra);
	objCobra.setPosX(1000);
	objCobra.setPoxY(200);

	/*
		4) Inicializar scene, backup, zBuffer e zBuffer2
	*/
	layers.at(4).addGameObject(&objPerAndando);
	layers.at(4).addGameObject(&objCobra);

	layers.at(0).computeScrollRateX(30);
	layers.at(1).computeScrollRateX(50);
	layers.at(2).computeScrollRateX(70);
	layers.at(3).computeScrollRateX(90);
	layers.at(4).computeScrollRateX(110);


	zBuffer = new char[1500 * 600];
	zBuffer2 = new char[1500 * 600];

	for (int i = 0; i < 1500 * 600; i++) {
	}
		zBuffer2[i] = 6;
		zBuffer[i] = 6;


}

void finalize(bool ganhou) {
	playing = false;
	glutHideWindow();
	cout << endl << endl << endl << endl << endl << endl << endl << endl;
	if (ganhou) {
		cout << "Parabens voce ganhou." << endl;
	}
	else {
		cout << "Voce perdeu." << endl;
	}
	dispose();
	cout << endl << endl << "Enter para sair" << endl;
	cin.get();
	exit(0);
}

void dispose() {
	delete zBuffer;
	delete zBuffer2;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	init();
	glutInitWindowSize(1500, 1500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Processamento Grafico - GA");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	update(1);
	glutMainLoop();

	return 0;

}