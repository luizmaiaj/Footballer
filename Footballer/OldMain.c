// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Mayor.h"

using namespace std;

//INCLUSAO DE BIBLIOTECAS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

struct tree {           //ESTRUTURA COM TOPO E TRES RAMIFICACOES

	char info;           //GUARDA APENAS UM CARACTERE REFERENTE A APENAS UM TERMINAL OU FUNCAO,
						 //VISA ECONOMIA DE MEMORIA E FACILIDADE DE IMPLEMENTACAO.
	struct tree* top;
	struct tree* right;
	struct tree* center;
	struct tree* left;
};

struct ind {            //INDIVIDUO COMPLETO COM ARVORE E FITNESS, UTILIZADO APENAS NA RAIZ
	signed long int fitness;
	struct tree* start;
};

struct robpos {         //POSICAO E DIRECAO DO ROBO
	int dir;
	double lin;
	double col;
};

struct ballpos {        //POSICAO E DIRECAO DA BOLA
	int dir;
	double lin;
	double col;
};

int n, nbef;                     //CONTROLA NUMERO DE SORTEIOS DO INDIVIDUOS

int enviro[HEIGHT][WIDTH],       //AMBIENTE
steps[HEIGHT][WIDTH],          //CAMINHO DO ROBO
ballsteps[HEIGHT][WIDTH];      //CAMINHO DA BOLA

int fit, unfit;                  //GUARDAM FITNESS

char pos;                        //GUARDA POSICAO DO RAMO DE TROCA

double dist[2];                  //DISTANCIAS

double Dini;                     //DISTANCIA INICIAL MEDIDA A CADA HIT

struct tree* ptr;                //RETORNO DE PONTO DE CRUZAMENTO

FILE* gene;

unsigned char ballmoves = 0, hits = 0;


//********************************
//* RECEBE RAIZ E LIBERA MEMORIA *
//* DE UM INDIVIDUO              *
//********************************
void freemem(struct tree* pointer)
{
	if (pointer->left)
		freemem(pointer->left);

	if (pointer->center)
		freemem(pointer->center);

	if (pointer->right)
		freemem(pointer->right);

	free(pointer);
}

//****************
//* MEDE FITNESS *
//****************
double fitness(struct robpos robot, struct ballpos ball)
{
	double distlin, distcol, distnow;
	double calc;


	distlin = ball.lin - robot.lin;
	distcol = ball.col - robot.col;

	distnow = sqrt((distcol * distcol) + (distlin * distlin));

	if (dist[0] == 0)
	{
		dist[0] = distnow;

		return(0);
	}
	else
	{
		dist[1] = dist[0];
		dist[0] = distnow;

		calc = 10 * (dist[1] - dist[0]);

		return((double)calc);
	}
}

//****************************
//* RECEBE DIRECAO DO ROBO E *
//* O DIRECIONA PARA BOLA    *
//****************************
void align(struct robpos* robot, struct ballpos ball)
{
	double Dlin, Dcol, angle;


	Dlin = ball.lin - robot->lin;
	Dcol = ball.col - robot->col;

	angle = (180 / PI) * atan(Dlin / Dcol);

	if (Dcol >= 0)
		robot->dir = 360 - angle;

	else
		robot->dir = 180 - angle;

	if (robot->dir >= 360)
		robot->dir -= 360;
}

//**************************************
//* RECEBE DADOS DO ROBO E SE ELE      *
//* ESTIVER PROXIMO A PAREDE RETORNA 1 *
//**************************************
int ifwall(struct robpos robot)
{
	double testlin, testcol;


	testlin = robot.lin - (2 * sin((PI * robot.dir) / 180));
	testcol = robot.col + (2 * cos((PI * robot.dir) / 180));

	if (enviro[(int)testlin][(int)testcol] || testlin < 1 || testlin > 198 || testcol < 1 || testlin > 198)
		return(1);

	return(0);
}

//*********************************
//* RECEBE DADOS DO ROBO E        *
//* VERIFICA SE ELE TOCOU NA BOLA *
//*********************************
void hitverify(struct robpos robot, struct ballpos* ball)
{
	double Dlin, Dcol;
	int stime;
	long ltime;


	//SEMENTE DO RANDOM
	ltime = time(NULL);
	stime = (unsigned)ltime / 2;
	srand(stime);

	Dlin = ball->lin - robot.lin;
	Dcol = ball->col - robot.col;

	if (sqrt((Dlin * Dlin) + (Dcol * Dcol)) <= HITDISTANCE)
	{
		//ballmoves = (rand() % 46) + 5;  //BALLMOVE RECEBE NUMERO DE 5 a 50;

		if (!nbef)
		{
			unfit = n / Dini;

			nbef = n;
		}
		else
		{
			unfit += (n - nbef) / Dini;

			nbef = n;
		}

		ballmoves = 40;
		ball->dir = robot.dir;
		hits++;
	}
}

//********************************
//* RECEBE DADOS DO ROBO E O     *
//* FAZ DAR UM PASSO PARA FRENTE *
//********************************
void walkfront(struct robpos* robot)
{
	double testlin, testcol;


	testlin = robot->lin - sin((PI * robot->dir) / 180);
	testcol = robot->col + cos((PI * robot->dir) / 180);

	if (!enviro[(int)testlin][(int)testcol])
	{
		enviro[(int)robot->lin][(int)robot->col] = 0;
		robot->lin = testlin;
		robot->col = testcol;
		enviro[(int)robot->lin][(int)robot->col] = 1;
	}
}

//******************************
//* RECEBE DADOS DO ROBO E O   *
//* FAZ DAR UM PASSO PARA TRAS *
//******************************
void walkback(struct robpos* robot)
{
	double testlin, testcol;


	testlin = robot->lin - sin((PI * (robot->dir + 180)) / 180);
	testcol = robot->col + cos((PI * (robot->dir + 180)) / 180);

	if (!enviro[(int)testlin][(int)testcol])
	{
		enviro[(int)robot->lin][(int)robot->col] = 0;
		robot->lin = testlin;
		robot->col = testcol;
		enviro[(int)robot->lin][(int)robot->col] = 1;
	}
}

//********************************
//* RECEBE DIRECAO DO ROBO E O   *
//* FAZ VIRAR `ANGLE` A ESQUERDA *
//********************************
void left(int* robotdir)
{
	*robotdir += ANGLE;

	if (*robotdir >= 360)
		*robotdir -= 360;
}

//*******************************
//* RECEBE DIRECAO DO ROBO E O  *
//* FAZ VIRAR `ANGLE` A DIREITA *
//*******************************
void right(int* robotdir)
{
	*robotdir -= ANGLE;

	if (*robotdir >= 360)
		*robotdir -= 360;
}

//********************************************
//* ALOCA MEMORIA PARA UMA ESTRUTURA         *
//* E RETORNA UM PONTEIRO PARA ESSA ALOCACAO *
//********************************************
struct tree* alloc(void)
{
	struct tree* pointer;


	pointer = (struct tree*) malloc(sizeof(struct tree));
	if (!pointer)
	{
		printf("\n\n\tFalha na alocacao de memoria!!!\n\n");
		exit(1);
	}

	return(pointer);
}

//*********************************
//* RECEBE PONTEIRO, SORTEIA INFO *
//* E O RETORNA                   *
//*********************************
struct tree* sort(struct tree* pointer)
{
	int randnum;       //GUARDA NUMERO SORTEADO
	struct tree* aux;  //AUXILIAR DE INICIALIZACAO DO PONTEIRO


	aux = alloc();

	aux->top = NULL;
	aux->left = NULL;
	aux->center = NULL;
	aux->right = NULL;

	if (n == 1)
		randnum = 1;     //NA PRIMEIRA EXECUCAO SORTEIA `PROGN3`

	else if (n < LIMIT)
		randnum = (rand() % 8) + 1;  //RANDNUM RECEBE NUMERO DE 1 a 8

	else randnum = (rand() % 4) + 5;  //SE EXCEDER `LIMIT` SORTEIOS PASSA A SORTEAR SOMENTE TERMINAIS

	n++;  //GUARDA COMPLEXIDADE DO INDIVIDUO

	switch (randnum)
	{
	case 1:
		pointer->info = 51;  //GUARDA `3` EM INFO REFERENTE A `PROGN3`
		break;
	case 2:
		pointer->info = 50;  //GUARDA `2` EM INFO REFERENTE A `PROGN2`
		break;
	case 3:
		pointer->info = 73;  //GUARDA `I` EM INFO REFERENTE A `IFWALL`
		break;
	case 4:
		pointer->info = 65;  //GUARDA `A` EM INFO REFERENTE A `ALIGN`
		break;
	case 5:
		pointer->info = 70;  //GUARDA `F` EM INFO REFERENTE A `WALKFRONT`
		break;
	case 6:
		pointer->info = 66;  //GUARDA `B` EM INFO REFERENTE A `WALKBACK`
		break;
	case 7:
		pointer->info = 76;  //GUARDA `L` EM INFO REFERENTE A `LEFT`
		break;
	case 8:
		pointer->info = 82;  //GUARDA `R` EM INFO REFERENTE A `RIGHT`
		break;
	}

	if (randnum > 3)
	{
		pointer->left = NULL;
		pointer->center = NULL;
		pointer->right = NULL;
	}
	else if (randnum == 2 || randnum == 3)
	{
		pointer->left = aux;
		pointer->center = NULL;
		pointer->right = aux;
	}
	else
	{
		pointer->left = aux;
		pointer->center = aux;
		pointer->right = aux;
	}

	freemem(aux);

	return(pointer);
}

//**************************************************
//* RECEBE PONTEIRO, CHAMA FUNCAO QUE SORTEIA INFO *
//* E O POSICIONA NA ARVORE                        *
//**************************************************
struct tree* maketree(struct tree* pointer)
{
	struct tree* aux;

	pointer = sort(pointer);  //RECEBE INFORMACAO SORTEADA

	if (pointer->left)  //CHECA VALIDADE DO PONTEIRO
	{
		aux = alloc();        //ALOCA aux

		pointer->left = aux;  //left APONTA aux

		aux->top = pointer;   //top APONTA pointer

		aux = maketree(aux);  //CONTINUA ARVORE
	}

	if (pointer->center)  //CHECA VALIDADE DO PONTEIRO
	{
		aux = alloc();          //ALOCA aux

		pointer->center = aux;  //center APONTA aux

		aux->top = pointer;     //top APONTA pointer

		aux = maketree(aux);    //CONTINUA ARVORE
	}

	if (pointer->right)  //CHECA VALIDADE DO PONTEIRO
	{
		aux = alloc();         //ALOCA aux

		pointer->right = aux;  //right APONTA aux

		aux->top = pointer;    //top APONTA pointer

		aux = maketree(aux);   //CONTINUA ARVORE
	}

	return(pointer);
}

//****************************************
//* RECEBE UM RAMO E UM PONTEIRO E       *
//* FAZ UMA COPIA DO SEGUNDO NO PRIMEIRO *
//****************************************
struct tree* copy(struct tree* pointer1, struct tree* pointer2)
{
	struct tree* aux;

	pointer1->info = pointer2->info;

	if (pointer2->left)
	{
		aux = alloc();

		pointer1->left = aux;

		aux->top = pointer1;

		copy(aux, pointer2->left);
	}
	else pointer1->left = NULL;

	if (pointer2->center)
	{
		aux = alloc();

		pointer1->center = aux;

		aux->top = pointer1;

		copy(aux, pointer2->center);
	}
	else pointer1->center = NULL;

	if (pointer2->right)
	{
		aux = alloc();

		pointer1->right = aux;

		aux->top = pointer1;

		copy(aux, pointer2->right);
	}
	else pointer1->right = NULL;

	return(pointer1);
}

//************************
//* RECEVE INDIVIDUO     *
//* E O IMPRIME NO VIDEO *
//************************
void print(struct tree* pointer)
{
	printf("%c ", pointer->info);

	if (pointer->left)
		print(pointer->left);

	if (pointer->center)
		print(pointer->center);

	if (pointer->right)
		print(pointer->right);
}

//*******************************************
//* RECEBE DADOS DA BOLA E DO ROBO E        *
//* MOVIMENTA BOLA DE ACORDO COM O AMBIENTE *
//*******************************************
void moveball(struct ballpos* ball, struct robpos robot)
{
	double testlin = 0, testcol = 0, Drest = 0, xi, xf, yi, yf, Dlin, Dcol;
	double lin, col;
	int stime, angle;
	long ltime;

	angle = ball->dir;
	lin = ball->lin;
	col = ball->col;

	while (ball->dir < 0)
		ball->dir += 360;

	while (ball->dir > 360)
		ball->dir -= 360;

	angle = ball->dir;

	//SEMENTE DO RANDOM
	ltime = time(NULL);
	stime = (unsigned)ltime / 2;
	srand(stime);

	if (ballmoves > 0)
	{
		ballmoves--;

		testlin = ball->lin - (2 * sin((PI * ball->dir) / 180));
		testcol = ball->col + (2 * cos((PI * ball->dir) / 180));

		if (testlin < 0 || testlin > 199 || testcol < 0 || testcol > 199)  //REFLEXAO EXTERNA
		{
			while (testlin < 0 || testlin > 199 || testcol < 0 || testcol > 199)
			{
				if (ball->dir == 90 || ball->dir == 270)
				{
					xi = xf = ball->col;
					yi = yf = -1;
				}
				else if (ball->dir == 0 || ball->dir == 180)
				{
					xi = xf = -1;
					yi = yf = ball->lin;
				}
				else
				{
					xi = ball->col + (ball->lin / tan((ball->dir * PI) / 180));
					xf = ball->col - ((199 - ball->lin) / tan((ball->dir * PI) / 180));

					yi = ball->lin + (tan((ball->dir * PI) / 180) * ball->col);
					yf = ball->lin - (tan((ball->dir * PI) / 180) * (199 - ball->col));
				}

				if (ball->dir > 0 && ball->dir < 180 && xi <= 199 && xi >= 0)
				{
					Dlin = ball->lin;
					Dcol = xi - ball->col;
					Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

					testlin = 1;
					testcol = xi;

					if (xi >= 198)
						ball->dir = 225;
					else if (xi <= 1)
						ball->dir = 315;
					else
						ball->dir = 360 - ball->dir;
				}
				else if (ball->dir < 270 && ball->dir > 90 && yi <= 199 && yi >= 0)
				{
					Dlin = yi - ball->lin;
					Dcol = ball->col;
					Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

					testlin = yi;
					testcol = 1;

					if (yi >= 198)
						ball->dir = 45;
					else if (yi <= 1)
						ball->dir = 315;
					else
						ball->dir = 180 - ball->dir;
				}
				else if (ball->dir < 360 && ball->dir > 180 && xf <= 199 && xf >= 0)
				{
					Dlin = 199 - ball->lin;
					Dcol = xf - ball->col;
					Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

					testlin = 198;
					testcol = xf;

					if (xf >= 198)
						ball->dir = 135;
					else if (xf <= 1)
						ball->dir = 45;
					else
						ball->dir = 360 - ball->dir;
				}
				else if (((ball->dir < 90 && ball->dir >= 0) || (ball->dir <= 360 && ball->dir > 270)) && yf <= 199 && yf >= 0)
				{
					Dlin = yf - ball->lin;
					Dcol = 199 - ball->col;
					Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

					testlin = yf;
					testcol = 198;

					if (yf >= 198)
						ball->dir = 135;
					else if (yf <= 1)
						ball->dir = 225;
					else
						ball->dir = 180 - ball->dir;
				}
				else
				{
					ball->dir -= 180;
				}

				while (ball->dir < 0)
					ball->dir += 360;

				while (ball->dir > 360)
					ball->dir -= 360;

				angle = ball->dir;

				testlin -= Drest * sin((PI * ball->dir) / 180);
				testcol += Drest * cos((PI * ball->dir) / 180);
			}

			Dlin = robot.lin - ball->lin;
			Dcol = robot.col - ball->col;
			Dini = sqrt((Dlin * Dlin) + (Dcol * Dcol));
		}
		else if (enviro[(int)testlin][(int)testcol] && (int)testlin < 197 && (int)testlin > 2 && (int)testcol < 197 && (int)testcol > 2)  //REFLEXAO INTERNA
		{
			if ((int)testlin < 66)
			{
				if ((int)testcol < 66)
				{
					if (ball->dir == 90 || ball->dir == 270)
					{
						xi = xf = ball->col;
						yi = yf = -1;
					}
					else if (ball->dir == 0 || ball->dir == 180)
					{
						xi = xf = -1;
						yi = yf = ball->lin;
					}
					else
					{
						xi = ball->col - ((25 - ball->lin) / tan((ball->dir * PI) / 180));
						xf = ball->col - ((40 - ball->lin) / tan((ball->dir * PI) / 180));

						yi = ball->lin - (tan((ball->dir * PI) / 180) * (25 - ball->col));
						yf = ball->lin - (tan((ball->dir * PI) / 180) * (40 - ball->col));
					}

					if (xi <= 40 && xi >= 25 && ball->dir < 360 && ball->dir > 180)
					{
						Dlin = 25 - ball->lin;
						Dcol = xi - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 24;
						testcol = xi;

						ball->dir = 360 - ball->dir;
					}
					else if (xf <= 40 && xf >= 25 && ball->dir < 180 && ball->dir > 0)
					{
						Dlin = 40 - ball->lin;
						Dcol = xf - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 41;
						testcol = xf;

						ball->dir = 360 - ball->dir;
					}
					else if (yi <= 40 && yi >= 25 && ((ball->dir < 90 && ball->dir >= 0) || (ball->dir <= 360 && ball->dir > 270)))
					{
						Dlin = yi - ball->lin;
						Dcol = 25 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yi;
						testcol = 24;

						ball->dir = 180 - ball->dir;
					}
					else if (yf <= 40 && yf >= 25 && ball->dir < 270 && ball->dir > 90)
					{
						Dlin = yf - ball->lin;
						Dcol = 40 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yf;
						testcol = 41;

						ball->dir = 180 - ball->dir;
					}
				}
				else if ((int)testcol < 132)
				{
					if (ball->dir == 90 || ball->dir == 270)
					{
						xi = xf = ball->col;
						yi = yf = -1;
					}
					else if (ball->dir == 0 || ball->dir == 180)
					{
						xi = xf = -1;
						yi = yf = ball->lin;
					}
					else
					{
						xi = ball->col - ((25 - ball->lin) / tan((ball->dir * PI) / 180));
						xf = ball->col - ((40 - ball->lin) / tan((ball->dir * PI) / 180));

						yi = ball->lin - (tan((ball->dir * PI) / 180) * (91 - ball->col));
						yf = ball->lin - (tan((ball->dir * PI) / 180) * (106 - ball->col));
					}

					if (xi <= 106 && xi >= 91 && ball->dir < 360 && ball->dir > 180)
					{
						Dlin = 25 - ball->lin;
						Dcol = xi - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 24;
						testcol = xi;

						ball->dir = 360 - ball->dir;
					}
					else if (xf <= 106 && xf >= 91 && ball->dir < 180 && ball->dir > 0)
					{
						Dlin = 40 - ball->lin;
						Dcol = xf - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 41;
						testcol = xf;

						ball->dir = 360 - ball->dir;
					}
					else if (yi <= 40 && yi >= 25 && ((ball->dir < 90 && ball->dir >= 0) || (ball->dir <= 360 && ball->dir > 270)))
					{
						Dlin = yi - ball->lin;
						Dcol = 91 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yi;
						testcol = 90;

						ball->dir = 180 - ball->dir;
					}
					else if (yf <= 40 && yf >= 25 && ball->dir < 270 && ball->dir > 90)
					{
						Dlin = yf - ball->lin;
						Dcol = 106 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yf;
						testcol = 107;

						ball->dir = 180 - ball->dir;
					}
				}
				else
				{
					if (ball->dir == 90 || ball->dir == 270)
					{
						xi = xf = ball->col;
						yi = yf = -1;
					}
					else if (ball->dir == 0 || ball->dir == 180)
					{
						xi = xf = -1;
						yi = yf = ball->lin;
					}
					else
					{
						xi = ball->col - ((25 - ball->lin) / tan((ball->dir * PI) / 180));
						xf = ball->col - ((40 - ball->lin) / tan((ball->dir * PI) / 180));

						yi = ball->lin - (tan((ball->dir * PI) / 180) * (160 - ball->col));
						yf = ball->lin - (tan((ball->dir * PI) / 180) * (175 - ball->col));
					}

					if (xi <= 175 && xi >= 160 && ball->dir < 360 && ball->dir > 180)
					{
						Dlin = 25 - ball->lin;
						Dcol = xi - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 24;
						testcol = xi;

						ball->dir = 360 - ball->dir;
					}
					else if (xf <= 175 && xf >= 160 && ball->dir < 180 && ball->dir > 0)
					{
						Dlin = 40 - ball->lin;
						Dcol = xf - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 41;
						testcol = xf;

						ball->dir = 360 - ball->dir;
					}
					else if (yi <= 40 && yi >= 25 && ((ball->dir < 90 && ball->dir >= 0) || (ball->dir <= 360 && ball->dir > 270)))
					{
						Dlin = yi - ball->lin;
						Dcol = 160 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yi;
						testcol = 159;

						ball->dir = 180 - ball->dir;
					}
					else if (yf <= 40 && yf >= 25 && ball->dir < 270 && ball->dir > 90)
					{
						Dlin = yf - ball->lin;
						Dcol = 175 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yf;
						testcol = 176;

						ball->dir = 180 - ball->dir;
					}
				}
			}
			else if ((int)testlin < 132)
			{
				if ((int)testcol < 66)
				{
					if (ball->dir == 90 || ball->dir == 270)
					{
						xi = xf = ball->col;
						yi = yf = -1;
					}
					else if (ball->dir == 0 || ball->dir == 180)
					{
						xi = xf = -1;
						yi = yf = ball->lin;
					}
					else
					{
						xi = ball->col - ((91 - ball->lin) / tan((ball->dir * PI) / 180));
						xf = ball->col - ((106 - ball->lin) / tan((ball->dir * PI) / 180));

						yi = ball->lin - (tan((ball->dir * PI) / 180) * (25 - ball->col));
						yf = ball->lin - (tan((ball->dir * PI) / 180) * (40 - ball->col));
					}

					if (xi <= 40 && xi >= 25 && ball->dir < 360 && ball->dir > 180)
					{
						Dlin = 91 - ball->lin;
						Dcol = xi - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 90;
						testcol = xi;

						ball->dir = 360 - ball->dir;
					}
					else if (xf <= 40 && xf >= 25 && ball->dir < 180 && ball->dir > 0)
					{
						Dlin = 106 - ball->lin;
						Dcol = xf - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 107;
						testcol = xf;

						ball->dir = 360 - ball->dir;
					}
					else if (yi <= 106 && yi >= 91 && ((ball->dir < 90 && ball->dir >= 0) || (ball->dir <= 360 && ball->dir > 270)))
					{
						Dlin = yi - ball->lin;
						Dcol = 25 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yi;
						testcol = 24;

						ball->dir = 180 - ball->dir;
					}
					else if (yf <= 106 && yf >= 91 && ball->dir < 270 && ball->dir > 90)
					{
						Dlin = yf - ball->lin;
						Dcol = 40 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yf;
						testcol = 41;

						ball->dir = 180 - ball->dir;
					}
				}
				else if ((int)testcol < 132)
				{
					if (ball->dir == 90 || ball->dir == 270)
					{
						xi = xf = ball->col;
						yi = yf = -1;
					}
					else if (ball->dir == 0 || ball->dir == 180)
					{
						xi = xf = -1;
						yi = yf = ball->lin;
					}
					else
					{
						xi = ball->col - ((91 - ball->lin) / tan((ball->dir * PI) / 180));
						xf = ball->col - ((106 - ball->lin) / tan((ball->dir * PI) / 180));

						yi = ball->lin - (tan((ball->dir * PI) / 180) * (91 - ball->col));
						yf = ball->lin - (tan((ball->dir * PI) / 180) * (106 - ball->col));
					}

					if (xi <= 106 && xi >= 91 && ball->dir < 360 && ball->dir > 180)
					{
						Dlin = 91 - ball->lin;
						Dcol = xi - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 90;
						testcol = xi;

						ball->dir = 360 - ball->dir;
					}
					else if (xf <= 106 && xf >= 91 && ball->dir < 180 && ball->dir > 0)
					{
						Dlin = 106 - ball->lin;
						Dcol = xf - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 107;
						testcol = xf;

						ball->dir = 360 - ball->dir;
					}
					else if (yi <= 106 && yi >= 91 && ((ball->dir < 90 && ball->dir >= 0) || (ball->dir <= 360 && ball->dir > 270)))
					{
						Dlin = yi - ball->lin;
						Dcol = 91 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yi;
						testcol = 90;

						ball->dir = 180 - ball->dir;
					}
					else if (yf <= 106 && yf >= 91 && ball->dir < 270 && ball->dir > 90)
					{
						Dlin = yf - ball->lin;
						Dcol = 106 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yf;
						testcol = 107;

						ball->dir = 180 - ball->dir;
					}
				}
				else
				{
					if (ball->dir == 90 || ball->dir == 270)
					{
						xi = xf = ball->col;
						yi = yf = -1;
					}
					else if (ball->dir == 0 || ball->dir == 180)
					{
						xi = xf = -1;
						yi = yf = ball->lin;
					}
					else
					{
						xi = ball->col - ((91 - ball->lin) / tan((ball->dir * PI) / 180));
						xf = ball->col - ((106 - ball->lin) / tan((ball->dir * PI) / 180));

						yi = ball->lin - (tan((ball->dir * PI) / 180) * (160 - ball->col));
						yf = ball->lin - (tan((ball->dir * PI) / 180) * (175 - ball->col));
					}

					if (xi <= 175 && xi >= 160 && ball->dir < 360 && ball->dir > 180)
					{
						Dlin = 91 - ball->lin;
						Dcol = xi - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 90;
						testcol = xi;

						ball->dir = 360 - ball->dir;
					}
					else if (xf <= 175 && xf >= 160 && ball->dir < 180 && ball->dir > 0)
					{
						Dlin = 106 - ball->lin;
						Dcol = xf - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 107;
						testcol = xf;

						ball->dir = 360 - ball->dir;
					}
					else if (yi <= 106 && yi >= 91 && ((ball->dir < 90 && ball->dir >= 0) || (ball->dir <= 360 && ball->dir > 270)))
					{
						Dlin = yi - ball->lin;
						Dcol = 160 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yi;
						testcol = 159;

						ball->dir = 180 - ball->dir;
					}
					else if (yf <= 106 && yf >= 91 && ball->dir < 270 && ball->dir > 90)
					{
						Dlin = yf - ball->lin;
						Dcol = 175 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yf;
						testcol = 176;

						ball->dir = 180 - ball->dir;
					}
				}
			}
			else
			{
				if ((int)testcol < 66)
				{
					if (ball->dir == 90 || ball->dir == 270)
					{
						xi = xf = ball->col;
						yi = yf = -1;
					}
					else if (ball->dir == 0 || ball->dir == 180)
					{
						xi = xf = -1;
						yi = yf = ball->lin;
					}
					else
					{
						xi = ball->col - ((160 - ball->lin) / tan((ball->dir * PI) / 180));
						xf = ball->col - ((175 - ball->lin) / tan((ball->dir * PI) / 180));

						yi = ball->lin - (tan((ball->dir * PI) / 180) * (25 - ball->col));
						yf = ball->lin - (tan((ball->dir * PI) / 180) * (40 - ball->col));
					}

					if (xi <= 40 && xi >= 25 && ball->dir < 360 && ball->dir > 180)
					{
						Dlin = 160 - ball->lin;
						Dcol = xi - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 159;
						testcol = xi;

						ball->dir = 360 - ball->dir;
					}
					else if (xf <= 40 && xf >= 25 && ball->dir < 180 && ball->dir > 0)
					{
						Dlin = 175 - ball->lin;
						Dcol = xf - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 176;
						testcol = xf;

						ball->dir = 360 - ball->dir;
					}
					else if (yi <= 175 && yi >= 160 && ((ball->dir < 90 && ball->dir >= 0) || (ball->dir <= 360 && ball->dir > 270)))
					{
						Dlin = yi - ball->lin;
						Dcol = 25 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yi;
						testcol = 24;

						ball->dir = 180 - ball->dir;
					}
					else if (yf <= 175 && yf >= 160 && ball->dir < 270 && ball->dir > 90)
					{
						Dlin = yf - ball->lin;
						Dcol = 40 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yf;
						testcol = 41;

						ball->dir = 180 - ball->dir;
					}
				}
				else if ((int)testcol < 132)
				{
					if (ball->dir == 90 || ball->dir == 270)
					{
						xi = xf = ball->col;
						yi = yf = -1;
					}
					else if (ball->dir == 0 || ball->dir == 180)
					{
						xi = xf = -1;
						yi = yf = ball->lin;
					}
					else
					{
						xi = ball->col - ((160 - ball->lin) / tan((ball->dir * PI) / 180));
						xf = ball->col - ((175 - ball->lin) / tan((ball->dir * PI) / 180));

						yi = ball->lin - (tan((ball->dir * PI) / 180) * (91 - ball->col));
						yf = ball->lin - (tan((ball->dir * PI) / 180) * (106 - ball->col));
					}

					if (xi <= 106 && xi >= 91 && ball->dir < 360 && ball->dir > 180)
					{
						Dlin = 160 - ball->lin;
						Dcol = xi - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 159;
						testcol = xi;

						ball->dir = 360 - ball->dir;
					}
					else if (xf <= 106 && xf >= 91 && ball->dir < 180 && ball->dir > 0)
					{
						Dlin = 175 - ball->lin;
						Dcol = xf - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 176;
						testcol = xf;

						ball->dir = 360 - ball->dir;
					}
					else if (yi <= 175 && yi >= 160 && ((ball->dir < 90 && ball->dir >= 0) || (ball->dir <= 360 && ball->dir > 270)))
					{
						Dlin = yi - ball->lin;
						Dcol = 91 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yi;
						testcol = 90;

						ball->dir = 180 - ball->dir;
					}
					else if (yf <= 175 && yf >= 160 && ball->dir < 270 && ball->dir > 90)
					{
						Dlin = yf - ball->lin;
						Dcol = 107 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yf;
						testcol = 107;

						ball->dir = 180 - ball->dir;
					}
				}
				else
				{
					if (ball->dir == 90 || ball->dir == 270)
					{
						xi = xf = ball->col;
						yi = yf = -1;
					}
					else if (ball->dir == 0 || ball->dir == 180)
					{
						xi = xf = -1;
						yi = yf = ball->lin;
					}
					else
					{
						xi = ball->col - ((160 - ball->lin) / tan((ball->dir * PI) / 180));
						xf = ball->col - ((175 - ball->lin) / tan((ball->dir * PI) / 180));

						yi = ball->lin - (tan((ball->dir * PI) / 180) * (160 - ball->col));
						yf = ball->lin - (tan((ball->dir * PI) / 180) * (175 - ball->col));
					}

					if (xi <= 175 && xi >= 160 && ball->dir < 360 && ball->dir > 180)
					{
						Dlin = 160 - ball->lin;
						Dcol = xi - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 159;
						testcol = xi;

						ball->dir = 360 - ball->dir;
					}
					else if (xf <= 175 && xf >= 160 && ball->dir < 180 && ball->dir > 0)
					{
						Dlin = 175 - ball->lin;
						Dcol = xf - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = 176;
						testcol = xf;

						ball->dir = 360 - ball->dir;
					}
					else if (yi <= 175 && yi >= 160 && ((ball->dir < 90 && ball->dir >= 0) || (ball->dir <= 360 && ball->dir > 270)))
					{
						Dlin = yi - ball->lin;
						Dcol = 160 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yi;
						testcol = 159;

						ball->dir = 180 - ball->dir;
					}
					else if (yf <= 175 && yf >= 160 && ball->dir < 270 && ball->dir > 90)
					{
						Dlin = yf - ball->lin;
						Dcol = 175 - ball->col;
						Drest = 2 - sqrt((Dlin * Dlin) + (Dcol * Dcol));

						testlin = yf;
						testcol = 176;

						ball->dir = 180 - ball->dir;
					}
				}
			}

			testlin -= Drest * sin((PI * ball->dir) / 180);
			testcol += Drest * cos((PI * ball->dir) / 180);

			Dlin = robot.lin - ball->lin;
			Dcol = robot.col - ball->col;
			Dini = sqrt((Dlin * Dlin) + (Dcol * Dcol));
		}

		if ((int)testlin == 0)
			testlin++;
		else if ((int)testlin == 199)
			testlin--;

		if ((int)testcol == 0)
			testcol++;
		else if ((int)testcol == 199)
			testcol--;

		enviro[(int)ball->lin][(int)ball->col] = 0;

		ball->lin = testlin;
		ball->col = testcol;

		ballsteps[(int)ball->lin][(int)ball->col] = 1;
		enviro[(int)ball->lin][(int)ball->col] = 1;
	}
}

//************************************
//* RECEBE DADOS DO ROBO E DA BOLA E UTILIZANDO OUTRAS FUNCOES SIMULA OS DOIS NO AMBIENTE
//************************************
void execute(struct tree* pointer, struct robpos* robot, struct ballpos* ball)
{
	moveball(ball, *robot);

	switch (pointer->info)
	{
	case 51:                    //PROGN3
		execute(pointer->left, robot, ball);    //CHAMA RAMO ESQUERDO

		execute(pointer->center, robot, ball);  //CHAMA RAMO CENTRAL

		execute(pointer->right, robot, ball);   //CHAMA RAMO DIREITO

		break;

	case 50:                    //PROGN2
		execute(pointer->left, robot, ball);

		execute(pointer->right, robot, ball);

		break;

	case 73:                    //IFWALL
		if (ifwall(*robot))
			execute(pointer->left, robot, ball);

		else
			execute(pointer->right, robot, ball);

		break;

	case 65:                    //ALIGN
		align(robot, *ball);

		steps[(int)robot->lin][(int)robot->col] = n;

		n++;

		break;

	case 70:                   //WALKFRONT
	{
		//int aux;


		walkfront(robot);

		//aux = fitness(*robot, *ball);

		//if(aux > 0)
		//  fit += aux;
		//else
		//  unfit -= aux;

		steps[(int)robot->lin][(int)robot->col] = n;

		n++;
	}

	hitverify(*robot, ball);

	break;

	case 66:                   //WALKBACK
	{
		//int aux;


		walkback(robot);

		//aux = fitness(*robot, *ball);

		//if(aux > 0)
		//  fit += aux;
		//else
		//  unfit -= aux;

		steps[(int)robot->lin][(int)robot->col] = n;

		n++;
	}

	hitverify(*robot, ball);

	break;

	case 76:                   //LEFT
		left(&robot->dir);

		steps[(int)robot->lin][(int)robot->col] = n;

		n++;

		break;

	case 82:                   //RIGHT
		right(&robot->dir);

		steps[(int)robot->lin][(int)robot->col] = n;

		n++;

		break;
	}
}

//*******************************
//* RECEBE MATRIZ E A PREENCHE CONFORME O AMBIENTE INICIAL
//*******************************
void setenviro(int matriz[HEIGHT][WIDTH])
{
	int lin, col;


	for (lin = 0; lin < HEIGHT; lin++)
		for (col = 0; col < WIDTH; col++)
		{
			if (lin == 0 || lin == 199 || col == 0 || col == 199)
				matriz[lin][col] = 1;

			else
				matriz[lin][col] = 0;
		}
}

//****************************************
//* RECEBE MATRIZ DE IMAGEM E A PREENCHE DE ACORDO COM O AMBIENTE INICIAL
//****************************************
void setbest(unsigned char matriz[HEIGHT][WIDTH][3])
{
	int lin, col;


	for (lin = 0; lin < HEIGHT; lin++)
		for (col = 0; col < WIDTH; col++)
		{
			if (lin == 0 || lin == 199 || col == 0 || col == 199)
				matriz[lin][col][0] = matriz[lin][col][1] = matriz[lin][col][2] = 255;

			else
				matriz[lin][col][0] = matriz[lin][col][1] = matriz[lin][col][2] = 0;
		}
}

//************************************
//* RECEBE INDIVIDUO E RETORNA O NUMERO DE PONTOS DE COMPLEXIDADE
//************************************
int length(struct tree* pointer)
{
	if (pointer->left)
	{
		length(pointer->left);
		n++;
	}

	if (pointer->center)
	{
		length(pointer->center);
		n++;
	}

	if (pointer->right)
	{
		length(pointer->right);
		n++;
	}

	return(n);
}

//****************************************************
//* RECEBE O INDIVIDUO E O PONTO DE CRUZAMENTO E RETORNA UM PONTEIRO PARA O PONTO DE CRUZAMENTO
//****************************************************
struct tree* count(struct tree* pointer)
{
	if (pointer->left)
	{
		n--;

		if (n > 0)
			count(pointer->left);

		else if (n == 0)
		{
			pos = 0;

			ptr = pointer->left;
		}
	}

	if (pointer->center)
	{
		n--;

		if (n > 0)
			count(pointer->center);

		else if (n == 0)
		{
			pos = 1;

			ptr = pointer->center;
		}
	}

	if (pointer->right)
	{
		n--;

		if (n > 0)
			count(pointer->right);

		else if (n == 0)
		{
			pos = 2;

			ptr = pointer->right;
		}
	}

	return ptr;
}

//********************************
//* RECEBE O INDIVIDUO E O SALVA EM UM ARQUIVO EM DISCO
//********************************
void save(struct tree* pointer)
{
	fprintf(gene, "%c", pointer->info);

	if (pointer->left)
		save(pointer->left);

	if (pointer->center)
		save(pointer->center);

	if (pointer->right)
		save(pointer->right);
}

//***************************************************
//* RECEBE PONTEIRO VAZIO, LE ARQUIVO DE UM INDIVIDUO E RETORNA O PONTEIRO COM A INFORMACAO
//***************************************************
struct tree* read(struct tree* pointer)
{
	struct tree* aux;
	char read_aux;

	aux = alloc();

	aux->top = NULL;
	aux->left = NULL;
	aux->center = NULL;
	aux->right = NULL;

	fscanf_s(gene, "%c", &read_aux);

	pointer->info = read_aux;

	if (pointer->info == 70 || pointer->info == 66 || pointer->info == 76 || pointer->info == 82 || pointer->info == 65)
	{
		pointer->left = NULL;
		pointer->center = NULL;
		pointer->right = NULL;
	}
	else if (pointer->info == 50 || pointer->info == 73)
	{
		pointer->left = aux;
		pointer->center = NULL;
		pointer->right = aux;
	}
	else
	{
		pointer->left = aux;
		pointer->center = aux;
		pointer->right = aux;
	}

	freemem(aux);

	return(pointer);
}

//****************************************************
//* RECEBE O PONTEIRO, CHAMA A FUNCAO READ, MONTA O INDIVIDUO E RETORNA O PONTEIRO DESSE INDIVIDUO
//****************************************************
struct tree* load(struct tree* pointer)
{
	struct tree* aux;

	pointer = read(pointer);

	if (pointer->left)
	{
		aux = alloc();

		pointer->left = aux;

		aux->top = pointer;

		aux = load(aux);
	}
	else pointer->left = NULL;

	if (pointer->center)
	{
		aux = alloc();

		pointer->center = aux;

		aux->top = pointer;

		aux = load(aux);
	}
	else pointer->center = NULL;

	if (pointer->right)
	{
		aux = alloc();

		pointer->right = aux;

		aux->top = pointer;

		aux = load(aux);
	}
	else pointer->right = NULL;

	return(pointer);
}

//*******************************************
//* DESENHA OBSTACULO NA MATRIZ DO AMBIENTE *
//*******************************************
void drawbox(int lin, int col, int size)
{
	int i, j;

	for (i = lin; i < (lin + size); i++)
		for (j = col; j < (col + size); j++)
			enviro[i][j] = 1;
}

//*****************************************
//* DESENHA OBSTACULO NA MATRIZ DA IMAGEM *
//*****************************************
void drawboxim(unsigned char matriz[HEIGHT][WIDTH][3], int lin, int col, int size)
{
	int i, j;

	for (i = lin; i < (lin + size); i++)
		for (j = col; j < (col + size); j++)
		{
			matriz[i][j][0] = matriz[i][j][1] = matriz[i][j][2] = 255;
		}
}

//************************
//*----------------------*
//*- PROGRAMA PRINCIPAL -*
//*----------------------*
//************************
int main(void)
{
	struct ind rob[POPULATION];
	struct ind robtemp[CROSSING];	//PONTEIRO INICIAL DO INDIVIDUO
	int stime;								//REPASSA A HORA PARA SEMENTE DO RANDOM
	long ltime;								//RECEBE A HORA PARA RANDOM
	int lin, col;                        //AUXILIARES DE IMPRESSAO DA MATRIZ
	int randnum;                         //AUXILIAR DE SORTEIO
	int i, j, k, gap, list[CROSSING],    //LOOPING E AUXILIAR DE SORTEIO
		crosspoint[2],                     //PONTOS DE CRUZAMENTO
		postree[2];                        //POSICAO DO RAMO
	struct ind x, parent[2];             //AUXILIARES DE ORGANIZACAO E CRUZAMENTO
	char a[5];
	struct tree* pointer[4];             //PONTEIROS PARA CRUZAMENTO
	signed long int best = 0;            //GUARDAM FITNESS
	unsigned char best_way[HEIGHT][WIDTH][3];
	struct robpos robot;
	struct ballpos ball;

	struct bests
	{
		unsigned char matrix[HEIGHT][WIDTH][3];  //MELHORES CAMINHOS DE CADA GERACAO
	}way[GENERATIONS];

	int gen, nsort, num;
	float gen_average, all_average = 0;
	int gen_maximum, all_maximum = 0;
	time_t begin = time(NULL); //HORA DO INICIO
	time_t end;

	//SEMENTE DO RANDOM
	ltime = time(NULL);
	stime = (unsigned)ltime / 2;
	srand(stime);

	//LIMPA TELA
	system("clear");
	printf("\n******************************************************************************\n");

	//---------------------------------------
	//ALOCA INICIO PARA INDIVIDUOS EM ARQUIVO
	//---------------------------------------
	printf("\n\t\tCarregando\n");
	for (i = 0; i < 100; i++)
	{
		char filename[20];

		rob[i].start = alloc();
		rob[i].start->top = NULL;
		rob[i].fitness = 0;

		sprintf_s(filename, "rb%.3dtr.txt", i);

		if (fopen_s(&gene, filename, "r"))
		{
			printf("\n\tNao existem sucessores para \"%s\"!!!", filename);
			printf("\n\tNovos individuos serao criados.\n\n");
			break;
		}

		rob[i].start = load(rob[i].start);

		fclose(gene);
	}

	//-------------------------------------------------------------
	//ALOCA INICIO PARA TODOS OS INDIVIDUOS E SORTEIA OS INDIVIDUOS
	//-------------------------------------------------------------
	printf("\n\t\tAlocando e Sorteando\n");
	for (; i < POPULATION; i++)
	{
		rob[i].start = alloc();    //ALOCA

		rob[i].start->top = NULL;

		rob[i].fitness = 0;

		n = 1;                            //SORTEIA
		rob[i].start = maketree(rob[i].start);

		if (i < CROSSING)
		{
			robtemp[i].start = alloc();

			robtemp[i].fitness = 0;
		}
	}

	//VERIFICA SE JA EXISTEM ARQUIVOS DE DADOS
	{
		char filename[20];


		i = 0;

		sprintf_s(filename, "data%.3d.txt", i);

		while (!fopen_s(&gene, filename, "r"))
		{
			fclose(gene);

			i++;
			sprintf_s(filename, "data%.3d.txt", i);
		}

		if (!fopen_s(&gene, filename, "w+"))  //ARQUIVO COM DADOS DA SIMULACAO
		{
			printf("\n\n\tFalha ao tentar criar arquivo!!!\n\n");
			exit(1);
		}

		else
		{
			fprintf(gene, "ROBO SEGUIDOR v.1.050\nLuiz Carlos Maia Junior\n");
			fprintf(gene, "GERACAO\tMEDIA\t\tMAIOR\n");
		}
	}

	//*******************
	//* INICIA GERACOES *
	//*******************
	for (gen = 0; gen < GENERATIONS; gen++)
	{
		gen_average = gen_maximum = 0;

		//EXECUCAO DO INDIVIDUO
		printf("\n\t\tExecutando ->       ");

		best = 0;

		for (i = 0; i < POPULATION; i++)
		{
			double aux, aux1;

			aux1 = i;

			aux = ((aux1 + 1) / POPULATION) * 100;

			printf("\b\b\b\b");

			if (aux >= 10)
				printf("\b");

			printf("%3.1f%%", aux);
			fflush(stdout);

			rob[i].fitness = 0;

			setenviro(enviro);

			drawbox(25, 25, 16);
			drawbox(25, 91, 16);
			drawbox(25, 160, 16);
			drawbox(91, 25, 16);
			drawbox(91, 91, 16);
			drawbox(91, 160, 16);
			drawbox(160, 25, 16);
			drawbox(160, 91, 16);
			drawbox(160, 160, 16);

			for (j = 0; j < RUNS; j++)
			{
				for (lin = 0; lin < HEIGHT; lin++)   //AMBIENTE DE PASSOS
					for (col = 0; col < WIDTH; col++)
						ballsteps[lin][col] = steps[lin][col] = 0;


				fit = unfit = hits = 0;

				//POSICAO E DIRECAO DO ROBO
				do
				{
					robot.dir = ANGLE * (rand() % (360 / ANGLE));

					robot.col = (int)(rand() % 198) + 1;

					robot.lin = (int)(rand() % 198) + 1;

					if (enviro[(int)robot.lin][(int)robot.col])
					{
						char sort;

						sort = (rand() % 2) + 1;

						switch (sort)
						{
						case 1:
							robot.col = (int)(rand() % 198) + 1;
							break;

						case 2:
							robot.lin = (int)(rand() % 198) + 1;
							break;
						}
					}

				} while (enviro[(int)robot.lin][(int)robot.col]);

				enviro[(int)robot.lin][(int)robot.col] = 1;

				//POSICAO DA BOLA
				do
				{
					ball.col = (int)(rand() % 198) + 1;

					ball.lin = (int)(rand() % 198) + 1;

					if (enviro[(int)ball.lin][(int)ball.col])
					{
						char sort;

						sort = (rand() % 2) + 1;

						switch (sort)
						{
						case 1:
							ball.col = (int)(rand() % 198) + 1;
							break;

						case 2:
							ball.lin = (int)(rand() % 198) + 1;
							break;
						}
					}

				} while (enviro[(int)ball.lin][(int)ball.col]);

				enviro[(int)ball.lin][(int)ball.col] = 1;

				steps[(int)robot.lin][(int)robot.col] = 1;
				ballsteps[(int)ball.lin][(int)ball.col] = 1;


				dist[0] = 0;

				{
					double Dlin, Dcol;

					nbef = 0;

					Dlin = ball.lin - robot.lin;
					Dcol = ball.col - robot.col;

					Dini = sqrt((Dlin * Dlin) + (Dcol * Dcol));
				}


				for (n = 0; n < EXECUTE;)
					execute(rob[i].start, &robot, &ball);

				//****************************************************
				//CALCULO DO FITNESS

				rob[i].fitness += 1000 * hits - unfit;
				//rob[i].fitness += 100 * hits - unfit;

				//printf("\n\n%d\n\n", unfit); fflush(stdout);

				//****************************************************

				if (rob[i].fitness >= best && j == (RUNS - 1))
				{
					setbest(best_way);    //MATRIZ QUE VAI VIRAR IMAGEM
					drawboxim(best_way, 25, 25, 16);
					drawboxim(best_way, 25, 91, 16);
					drawboxim(best_way, 25, 160, 16);
					drawboxim(best_way, 91, 25, 16);
					drawboxim(best_way, 91, 91, 16);
					drawboxim(best_way, 91, 160, 16);
					drawboxim(best_way, 160, 25, 16);
					drawboxim(best_way, 160, 91, 16);
					drawboxim(best_way, 160, 160, 16);

					best = rob[i].fitness;

					for (lin = 0; lin < HEIGHT; lin++)
						for (col = 0; col < WIDTH; col++)
						{
							if (steps[lin][col] > 0)
							{
								if (steps[lin][col] < 20)
								{
									best_way[lin][col][0] = 255;
									best_way[lin][col][1] = 255;
									best_way[lin][col][2] = 0;
								}
								else
								{
									best_way[lin][col][0] = 255;
									best_way[lin][col][1] = 0;
									best_way[lin][col][2] = 0;
								}
							}
							if (ballsteps[lin][col] == 1)
							{
								best_way[lin][col][0] = 0;
								best_way[lin][col][1] = 255;
								best_way[lin][col][2] = 0;
							}
						}
				}
			}

			rob[i].fitness /= j;
		}

		//*********************************
		//* ORGANIZA EM ORDEM DECRESCENTE *
		//*********************************
		printf("\n\n\t\tOrganizando\n");

		a[0] = 7;
		a[1] = 5;
		a[2] = 3;
		a[3] = 2;
		a[4] = 1;

		for (k = 0; k < 5; k++)
		{
			gap = a[k];
			for (i = gap; i < POPULATION; ++i)
			{
				x = rob[i];
				for (j = i - gap; x.fitness > rob[j].fitness && j >= 0; j = j - gap)
					rob[j + gap] = rob[j];
				rob[j + gap] = x;
			}
		}

		//**************            
		//* CRUZAMENTO *
		//**************
		if (gen < (GENERATIONS - 1))
		{
			printf("\n\t\tCruzando\n");

			for (i = 0; i < CROSSING; i++)
				list[i] = i;

			nsort = CROSSING;

			for (num = 0; num < CROSSING; num += 2)
			{
				for (i = 0; i < 2; i++)  //SORTEIO DOS PAIS
				{
					if (nsort > 0)
						randnum = rand() % nsort;

					else randnum = 0;

					parent[i].start = alloc();

					parent[i].start = copy(parent[i].start, rob[list[randnum]].start);

					nsort--;

					list[randnum] = list[nsort];

					n = 1;

					crosspoint[i] = (rand() % (length(parent[i].start) - 1)) + 2;
				}

				for (i = 0; i < 2; i++)
				{
					n = crosspoint[i] - 1;

					count(parent[i].start);

					pointer[i] = ptr;

					postree[i] = pos;
				}

				for (i = 0; i < 2; i++)
					pointer[i + 2] = pointer[i]->top;

				switch (postree[0])
				{
				case 0:
					pointer[2]->left = pointer[1];

					pointer[1]->top = pointer[2];
					break;

				case 1:
					pointer[2]->center = pointer[1];

					pointer[1]->top = pointer[2];
					break;

				case 2:
					pointer[2]->right = pointer[1];

					pointer[1]->top = pointer[2];
					break;
				}

				switch (postree[1])
				{
				case 0:
					pointer[3]->left = pointer[0];

					pointer[0]->top = pointer[3];
					break;

				case 1:
					pointer[3]->center = pointer[0];

					pointer[0]->top = pointer[3];
					break;

				case 2:
					pointer[3]->right = pointer[0];

					pointer[0]->top = pointer[3];
					break;
				}

				robtemp[num].start = parent[0].start;
				robtemp[num + 1].start = parent[1].start;
			}

			/*
			  -----------------------
			  MANUSEIO DE MEMORIA
			  -----------------------
				*/
				//printf("\n\t\tManuseio de memoria\n");

			for (num = 0; num < CROSSING; num++)
			{
				//freemem(rob[REPRODUCTION + num].start);     //LIBERA 'ROB'

				//rob[REPRODUCTION + num].start = alloc();    //REALOCA 'ROB'
				//rob[REPRODUCTION + num].start->top = NULL;

				rob[REPRODUCTION + num].start = robtemp[num].start;

				//freemem(robtemp[num].start);          //LIBERA INICIO DE 'TEMP'

				//robtemp[num].start = alloc();         //REALOCA 'TEMP'
				//robtemp[num].fitness = 0;
			}

		}

		printf("\n\t\tGeracao: %d", gen + 1);

		for (num = 0; num < POPULATION; num++)
		{
			gen_average += rob[num].fitness;

			if (rob[num].fitness > gen_maximum)
				gen_maximum = rob[num].fitness;

			if (gen_maximum > all_maximum)
				all_maximum = gen_maximum;
		}

		gen_average /= POPULATION;

		all_average += gen_average;

		printf("\n\n\tMEDIA GERACAO = %3.4f, MAIOR GERACAO = %3d\n", gen_average, gen_maximum);

		//IMPRIME INDIVIDUO, FITNESS E COMPLEXIDADE
		for (i = 0; i < 10; i++)
		{
			n = 1;

			printf("\n");

			printf("\n\t%.2d - FITNESS = %3d, e COMPRIMENTO = %3d\n", i, rob[i].fitness, length(rob[i].start));
		}

		printf("\n");

		fprintf(gene, "%d\t%3.4f  \t%4d\n", gen + 1, gen_average, gen_maximum);
		fflush(gene);

		printf("\n******************************************************************************\n");

		for (lin = 0; lin < HEIGHT; lin++)
			for (col = 0; col < WIDTH; col++)
				for (i = 0; i < 3; i++)
					way[gen].matrix[lin][col][i] = best_way[lin][col][i];
	}

	all_average /= (GENERATIONS - 1);

	printf("\n\tMEDIA GERAL = %3.2f, MAIOR GERAL = %4d\n", all_average, all_maximum);

	fflush(gene);
	fclose(gene);

	//VERIFICA ARQUIVOS DE INDIVIDUOS QUE JA EXISTEM
	{
		char filename[20];


		i = 0;

		sprintf_s(filename, "rb%.3d.tr", i);

		while (!fopen_s(&gene, filename, "r"))
		{
			fclose(gene);

			i++;
			sprintf_s(filename, "rb%.3d.tr", i);
		}
	}

	//********************
	//* SALVA INDIVIDUOS *
	//********************
	printf("\n\t\tSalvando individuos...\n");
	{
		int aux;

		for (aux = 0; aux < 100; aux++, i++)
		{
			char filename[20];

			sprintf_s(filename, "rb%.3dtr.txt", i);

			if (!fopen_s(&gene, filename, "w+"))
			{
				save(rob[aux].start);

				n = 1;
				fprintf(gene, "\n\nCOMPRIMENTO = %d\n\nFITNESS = %d\n", length(rob[i].start), rob[i].fitness);
				fclose(gene);
			}
		}
	}

	//VERIFICA ARQUIVOS DE IMAGEM QUE JA EXISTEM
	{
		char filename[20];


		i = 0;

		sprintf_s(filename, "caminho%.3d.ppm", i);

		while (!fopen_s(&gene, filename, "r"))
		{
			fclose(gene);

			i++;
			sprintf_s(filename, "caminho%.3d.ppm", i);
		}
	}

	//******************
	//* SALVA CAMINHOS *
	//****************** 
	printf("\n\t\tSalvando caminhos ->      ");
	for (gen = 0; gen < GENERATIONS; gen++, i++)
	{
		char filename[20];
		double aux, aux1;

		aux = ((gen + 1) / GENERATIONS) * 100;

		sprintf_s(filename, "caminho%.3d.ppm", i);

		//SALVA UMA MATRIZ PARA UM ARQUIVO TIPO PPM

		if (fopen_s(&gene, filename, "w+"))
		{
			printf("\n\n\tArquivo nao pode ser criado.\n\n");
			exit(-1);
		}

		fprintf(gene, "P3 \n 200 200 \n 255 \n");  //GRAVA COMECO

		for (lin = 0; lin < HEIGHT; lin++)     //GRAVA NO ARQUIVO
		{
			for (col = 0; col < WIDTH; col++)
				fprintf(gene, "%d %d %d   ", way[gen].matrix[lin][col][0], way[gen].matrix[lin][col][1], way[gen].matrix[lin][col][2]);

			fprintf(gene, "\n");
		}

		fclose(gene);  //FECHA ARQUIVO

		//sprintf_s(command, "convert %s caminho%.3d.gif", filename, i);

		//system(command);

		//sprintf_s(command, "rm %s -f", filename);

		//system(command);

		aux1 = gen;

		aux = ((aux1 + 1) / GENERATIONS) * 100;

		printf("\b\b\b\b");

		if (aux >= 10)
			printf("\b");

		printf("%3.1f%%", aux); fflush(stdout);
	}

	//******************
	//* LIBERA MEMORIA *
	//******************
	printf("\n\n\t\tLiberando memoria...\n");
	for (i = 0; i < POPULATION; i++)
		freemem(rob[i].start);

	//HORA DO FIM
	end = time(NULL);

	{
		double timespent;
		int hours;
		int mins;
		int secs;

		timespent = difftime(end, begin);

		hours = (int)timespent / 3600;

		timespent -= 3600 * hours;

		mins = (int)timespent / 60;

		timespent -= 60 * mins;

		secs = (int)timespent;

		printf("\n\n\tTempo Gasto %.2d:%.2d:%.2d\n\n", hours, mins, secs);

		printf("\n\n\t\"FIM\"\n\n\a");
	}

	//SINAL DE SAIDA
	return(0);
}
