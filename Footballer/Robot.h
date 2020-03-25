#pragma once
#include "Tree.h"

enum class Direction{UP, DOWN, LEFT, RIGHT};

class Robot
{
	long m_fitness{ 0 };
	Tree* m_start{ nullptr };

	Direction m_dir{ Direction::UP };
	double m_lin{ 0 };
	double m_col{ 0 };
};

