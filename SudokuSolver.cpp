#include "SudokuSolver.h"
//#include <iostream>


SudokuSolver::SudokuSolver()
{


}

void SudokuSolver::extractMatrix(Matrix Sudoku)
{

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			
			this->tab[i][j] = Sudoku.at(i, j);
			
		}
	}
	
}
void SudokuSolver::extractArray(int tab[N][N]) {
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			SudokuMatrix.at(i, j) = tab[i][j];
		}
	}
}
