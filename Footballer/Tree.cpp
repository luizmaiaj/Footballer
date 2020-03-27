#include "Tree.h"

//****************************************************
//* RECEBE O PONTEIRO, CHAMA A FUNCAO READ, MONTA O INDIVIDUO E RETORNA O PONTEIRO DESSE INDIVIDUO
//****************************************************
bool Tree::load(string aFilename)
{
	ifstream inputFile(aFilename);
	if (inputFile.is_open())
	{
		inputFile >> m_ssFile;
		inputFile.close();

		load(this);

		return true;
	}

	return false;
}

void Tree::create(Tree* pTree, uint aID)
{
	uint randnum{ 0 };       //GUARDA NUMERO SORTEADO

	m_id = aID;
	m_top = pTree;

	if (m_root)						//NA PRIMEIRA EXECUCAO SORTEIA `PROGN3`
		randnum = 1;
	else if (m_id < LIMIT)			//RANDNUM RECEBE NUMERO DE 1 a 8
		randnum = (rand() % 8) + 1;
	else							//SE EXCEDER `LIMIT` SORTEIOS PASSA A SORTEAR SOMENTE TERMINAIS
		randnum = (rand() % 4) + 5;  

	//printf("%i ", randnum);

	switch (randnum)
	{
	case 1:
		m_info = LEAF::PROGN3; //51;  //GUARDA `3` EM INFO REFERENTE A `PROGN3`
		m_left = new Tree();
		m_left->create(this, ++m_id);
		m_center = new Tree();
		m_center->create(this, ++m_id);
		m_right = new Tree();
		m_right->create(this, ++m_id);
		break;
	case 2:
		m_info = LEAF::PROGN2; // 50;  //GUARDA `2` EM INFO REFERENTE A `PROGN2`
		m_left = new Tree();
		m_left->create(this, ++m_id);
		m_right = new Tree();
		m_right->create(this, ++m_id);
		break;
	case 3:
		m_info = LEAF::IFWALL; // 73;  //GUARDA `I` EM INFO REFERENTE A `IFWALL`
		m_left = new Tree();
		m_left->create(this, ++m_id);
		m_right = new Tree();
		m_right->create(this, ++m_id);
		break;
	case 4:
		m_info = LEAF::ALIGN; // 65;  //GUARDA `A` EM INFO REFERENTE A `ALIGN`
		break;
	case 5:
		m_info = LEAF::FRONT; // 70;  //GUARDA `F` EM INFO REFERENTE A `WALKFRONT`
		break;
	case 6:
		m_info = LEAF::BACK; // 66;  //GUARDA `B` EM INFO REFERENTE A `WALKBACK`
		break;
	case 7:
		m_info = LEAF::LEFT; // 76;  //GUARDA `L` EM INFO REFERENTE A `LEFT`
		break;
	case 8:
		m_info = LEAF::RIGHT; // 82;  //GUARDA `R` EM INFO REFERENTE A `RIGHT`
		break;
	}
}

void Tree::setRoot(bool aRoot)
{
	m_root = aRoot;
}

bool Tree::getRoot()
{
	return m_root;
}

Tree* Tree::load(Tree* pointer)
{
	return(pointer);
}