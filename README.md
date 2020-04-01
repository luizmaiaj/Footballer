# Footballer
Genetically programmed footballer

THE DESCRIPTION BELOW IS TO BE UPDATED

/*
 ******************************************************************************************************
 SEGUIR BOLA, BOLA REFLETE COM DISTANCIA CONSTANTE, FUNCAO DE DESVIO DE PAREDES, FITNESS COM HITS
 ******************************************************************************************************

 -------------------------------------------------------------
 BASEADO EM ALOCACAO DINAMICA DE MEMORIA BUSCA NODE LEFT-RIGHT
 -------------------------------------------------------------

  OBJETIVO:
	INDIVIDUO DEVE PERCORRER A MATRIZ E ALCANCAR A BOLA SE MOVIMENTANDO O MINIMO POSSIVEL, E DESVIANDO
	DOS OBSTACULOS.
	AS POSICOES INICIAIS TANTO DA BOLA COMO DO ROBO SAO ALEATORIAS. O ROBO NAO AGUARDA A BOLA PARAR
	NOVAMENTE PARA SEGUI-LA.

  FITNESS:
	SERA LEVADO EM CONSIDERACAO A DISTANCIA INICIAL ENTRE O ROBO E A BOLA INICIAL E DEPOIS DE CADA TOQUE,
	O NUMERO DE PASSOS DO ROBO E O NUMERO DE VEZES QUE ELE TOCOU A BOLA.

  FUNCOES:
	PROGN3  (3), EXECUTA TRES RAMOS EM SEQUENCIA;
	PROGN2  (2), EXECUTA DOIS RAMOS EM SEQUENCIA;
	IFWALL  (I), EXECUTA O RAMO ESQUERDO SE HOUVER PAREDE E VICE-VERSA.

  TERMINAIS:
	WALKFRONT (F), FAZ ROBO ANDAR PRA FRENTE;
	WALKBACK  (B), FAZ ROBO ANDAR PRA TRAS;
	RIGHT     (R), FAZ ROBO VIRAR A DIREITA (DEPENDE DE `ANGLE`);
	LEFT      (L), FAZ ROBO VIRAR A ESQUERDA (DEPENDE DE `ANGLE`);
	ALIGN     (A), DIRECIONA ROBO PARA A BOLA (NAO DEPENDE DE `ANGLE`).

  ----------------------------------------------------------
  PARAMETROS IMPORTANTES:
	NUMERO DE GERACOES(G): DEFINIDO PELO PARAMETRO `GENERATIONS`.
	TAMANHO DA POPULACAO(M): DEFINIDO PELO PARAMETRO `POPULATION`.
	PROBABILIDADE DE REPRODUCAO = DEFINIDA PELO PARAMETRO `REPRODUCTION`.
	PROBABILIDADE DE CRUZAMENTO = DEFINIDA PELO PARAMETRO `CROSSING`.
	PROBABILIDADE DE MUTACAO    =  0%
  ----------------------------------------------------------

  OUTROS DADOS:
	COMPLEXIDADE DOS INDIVIDUOS E LIMITADA (PARAMETRO `LIMIT`).
*/
