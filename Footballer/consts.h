#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;

constexpr uint GENERATIONS = 50;							//NUMERO DE GERACOES;
constexpr uint POPULATION = 50;							//TAMANHO DA POPULACAO;
constexpr uint CROSSING = (uint)(POPULATION * 0.7f); //70% DA POPULACAO;
constexpr uint REPRODUCTION = POPULATION - CROSSING;        //30% DA POPULACAO;
constexpr uint HEIGHT = 200;								//ALTURA DA MATRIZ;
constexpr uint WIDTH = 200;									//LARGURA DA MATRIZ;
constexpr uint RUNS = 1;									//Number of times that each individual is executed
constexpr uint MOVES = 2000;								//Number of times that the execution will move through the tree
constexpr uint LIMIT = 1000;								//LIMITA COMPRIMENTO DO INDIVIDUO;
constexpr float ANGLE = 5.f;										//ANGULO QUE O ROBO SE VIRA;
constexpr uint M_360_ANGLE = (uint) (360 / ANGLE);			//ANGULO QUE O ROBO SE VIRA;
constexpr uint HITDISTANCE = 1;								//DISTANCIA CONSIDERADA PARA TOQUE;

constexpr double C_PI = 3.14159265358979323846; // pi;
constexpr double C_PI_180 = C_PI / 180; // pi divided by 180 degrees
constexpr double C_180_PI = 180 / C_PI; // 180 degrees divided by pi

enum class LEAF {PROGN3, PROGN2, IFWALL, ALIGN, FRONT, BACK, LEFT, RIGHT};
enum class WALL { TOP, BOTTOM, LEFT, RIGHT, TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT };
enum class STATE {WAITING, SIMULATING, CROSSING};