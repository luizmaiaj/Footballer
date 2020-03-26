#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

typedef unsigned int uint;

constexpr unsigned int GENERATIONS = 50;							//NUMERO DE GERACOES;
constexpr unsigned int POPULATION = 10;							//TAMANHO DA POPULACAO;
constexpr unsigned int CROSSING = (unsigned int)(POPULATION * 0.7f); //70% DA POPULACAO;
constexpr unsigned int REPRODUCTION = POPULATION - CROSSING;        //30% DA POPULACAO;
constexpr unsigned int HEIGHT = 200;								//ALTURA DA MATRIZ;
constexpr unsigned int WIDTH = 200;									//LARGURA DA MATRIZ;
constexpr unsigned int RUNS = 1;									//NUMERO DE TESTES DE CADA INDIVIDUOS;
constexpr unsigned int EXECUTE = 2000;								//NUMERO DE EXECUCOES DA ARVORE POR TESTE;
constexpr unsigned int LIMIT = 1000;								//LIMITA COMPRIMENTO DO INDIVIDUO;
constexpr unsigned int ANGLE = 5;									//ANGULO QUE O ROBO SE VIRA;
constexpr unsigned int HITDISTANCE = 1;								//DISTANCIA CONSIDERADA PARA TOQUE;

constexpr double PI = 3.14159265358979323846; //PI;
