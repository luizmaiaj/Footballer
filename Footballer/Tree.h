#pragma once

class Tree
{
	char info{ '0' }; // character with the action or function to perform

	Tree* m_top{ nullptr };
	Tree* m_left{ nullptr };
	Tree* m_center{ nullptr };
	Tree* m_right{ nullptr };
};

