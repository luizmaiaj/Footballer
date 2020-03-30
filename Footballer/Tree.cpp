#include "Tree.h"

Tree::~Tree()
{
	if (m_left) delete m_left;
	if (m_center) delete m_center;
	if (m_right) delete m_right;
}

// RECEBE O PONTEIRO, CHAMA A FUNCAO READ, MONTA O INDIVIDUO E RETORNA O PONTEIRO DESSE INDIVIDUO
bool Tree::load(string aFilename)
{
	return false;
}

void Tree::create(Tree* pTree, unsigned long &aSize, Tree* pParent)
{
	m_id = ++aSize;
	m_top = pTree;

	m_info = (pParent) ? pParent->getInfo() : randomLeaf();

	//printf("%d:%d, ", m_id, m_info);

	switch (m_info)
	{
	case LEAF::PROGN3:
		m_left = new Tree();
		m_left->create(this, aSize, (pParent) ? pParent->m_left : nullptr);
		m_center = new Tree();
		m_center->create(this, aSize, (pParent) ? pParent->m_center : nullptr);
		m_right = new Tree();
		m_right->create(this, aSize, (pParent) ? pParent->m_right : nullptr);
		break;
	case LEAF::PROGN2:
		m_left = new Tree();
		m_left->create(this, aSize, (pParent) ? pParent->m_left : nullptr);
		m_right = new Tree();
		m_right->create(this, aSize, (pParent) ? pParent->m_right : nullptr);
		break;
	case LEAF::IFWALL:
		m_left = new Tree();
		m_left->create(this, aSize, (pParent) ? pParent->m_left : nullptr);
		m_right = new Tree();
		m_right->create(this, aSize, (pParent) ? pParent->m_right : nullptr);
		break;
	default:
		break;
	}
}

LEAF Tree::randomLeaf()
{
	//srand((unsigned int)time(0));

	LEAF randLeaf{ LEAF::PROGN3 };

	if (m_id == 1)						//NA PRIMEIRA EXECUCAO SORTEIA `PROGN3`
		randLeaf = (LEAF) 0;
	else if (m_id < LIMIT)			//RANDNUM RECEBE NUMERO DE 1 a 8
		randLeaf = (LEAF) ((rand() % 8) + 1);
	else							//SE EXCEDER `LIMIT` SORTEIOS PASSA A SORTEAR SOMENTE TERMINAIS
		randLeaf = (LEAF) ((rand() % 4) + 5);

	return randLeaf;
}

Tree* Tree::getPoint(unsigned long aPoint)
{
	if (m_id == aPoint)
		return this;

	Tree* pTemp{ nullptr };

	if (m_left)
		if (pTemp = m_left->getPoint(aPoint))
			return pTemp;
	
	if (m_center)
		if (pTemp = m_center->getPoint(aPoint))
			return pTemp;

	if (m_right)
		if (pTemp = m_right->getPoint(aPoint))
			return pTemp;

	return nullptr;
}

Tree* Tree::setPoint(Tree* pPoint, unsigned long aPoint)
{
	if (m_id == aPoint)
		return this;

	Tree* pTemp{ nullptr };

	if (m_left)
		if (pTemp = m_left->getPoint(aPoint))
		{
			m_left = pTemp;
			return nullptr;
		}

	if (m_center)
		if (pTemp = m_center->getPoint(aPoint))
		{
			m_center = pTemp;
			return nullptr;
		}

	if (m_right)
		if (pTemp = m_right->getPoint(aPoint))
		{
			m_right = pTemp;
			return nullptr;
		}

	return nullptr;
}

void Tree::setRoot(bool aRoot)
{
	m_root = aRoot;
}

bool Tree::getRoot()
{
	return m_root;
}

LEAF Tree::getInfo()
{
	return m_info;
}

Tree* Tree::getNext(LEAF aDirection)
{
	if (!m_bRun)
		return this;

	if (aDirection == LEAF::RIGHT)
		m_left->reset(true);
	else if (aDirection == LEAF::LEFT)
		m_right->reset(true);


	if (m_left)
		if (!m_left->wasRun())
			return m_left->getNext(aDirection);
	if (m_center)
		if (!m_center->wasRun())
			return m_center->getNext(aDirection);
	if (m_right)
		if (!m_right->wasRun())
			return m_right->getNext(aDirection);

	return m_top;
}

void Tree::run()
{
	m_bRun = true;
}

bool Tree::wasRun()
{
	return m_bRun;
}

void Tree::reset(bool bResetTo)
{
	if (m_left) m_left->reset(bResetTo);
	if (m_center) m_center->reset(bResetTo);
	if (m_right) m_right->reset(bResetTo);

	m_bRun = bResetTo;
}

Tree* Tree::load(Tree* pointer)
{
	return(pointer);
}