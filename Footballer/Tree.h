#pragma once
#include "consts.h"

using namespace std;

class Tree
{
public:
	bool load(string aFilename);
	void create(Tree* pTree, uint aID);
	void setRoot(bool aRoot);
	bool getRoot();

private:
	Tree* load(Tree* pointer);

	LEAF m_info{ LEAF::PROGN3 }; // character with the action or function to perform

	Tree* m_top{ nullptr };
	Tree* m_left{ nullptr };
	Tree* m_center{ nullptr };
	Tree* m_right{ nullptr };

	bool m_root{ false }; // determines if this is the root of the tree

	uint m_id{ 0 }; // the sequential number of the branch or leaf when it was created

	string m_ssFile;
};

