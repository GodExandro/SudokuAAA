#pragma once
//#include "SolverAlgorithm.cpp"
#include "Matrix.h"
using namespace std;
#define N 9
class SudokuSolver {
	
	
public:
		SudokuSolver() ;

		int tab[N][N];
		Matrix SudokuMatrix;

		
		void extractArray(int tab[N][N]);
		void extractMatrix(Matrix);
		bool SolveSudoku(int grid[N][N]);
		void printSu(int grid[N][N]);

};