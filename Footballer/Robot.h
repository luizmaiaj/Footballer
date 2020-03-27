#pragma once
#include "Tree.h"
#include "City.h"

class Robot
{
public:
	~Robot();
	void birth();

private:
	long m_fitness{ 0 };
	Tree* m_start{ nullptr };
	uint m_dir{ 0 };
	uint m_lin{ 0 };
	uint m_col{ 0 };
};

