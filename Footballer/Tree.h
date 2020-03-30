#pragma once
#include "consts.h"

class Tree
{
public:
	~Tree();
	bool load(string aFilename);
	void create(Tree* pTree, unsigned long& aSize, Tree* pParent);
	Tree* getPoint(unsigned long aPoint);
	Tree* setPoint(Tree* pPoint, unsigned long aPoint);
	void setRoot(bool aRoot);
	bool getRoot();
	LEAF getInfo();
	Tree* getNext(LEAF aDirection);
	void run();
	bool wasRun();
	void reset(bool bResetTo);
	LEAF randomLeaf();

private:
	Tree* load(Tree* pointer);

	LEAF m_info{ LEAF::PROGN3 }; // character with the action or function to perform

	Tree* m_top{ nullptr };
	Tree* m_left{ nullptr };
	Tree* m_center{ nullptr };
	Tree* m_right{ nullptr };

	bool m_root{ false }; // determines if this is the root of the tree

	unsigned long m_id{ 0 }; // the sequential number of the branch or leaf when it was created

	bool m_bRun{ false };
};

