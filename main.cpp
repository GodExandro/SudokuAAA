#include "ImageProcessing.h"
#include "SudokuNumbers.h"
#include <string>
#include <iostream>
#include "KNN.h"
#include "SudokuSolver.h"

using namespace std;

#define N 9

bool SolveSudoku(int grid[N][N]);
void printSu(int grid[N][N]);

int main() {
	ImageProcessing image;
	string filename = "Resources/111.jpg";
	Matrix Sudoku(9,9);
	image.loadImage(filename);
	image.process();

	SudokuNumbers sn(image.getImage());
	Sudoku = sn.extractNumbers();
	cout << "=========Extracted matrix==========" << endl;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			cout << Sudoku.at(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl << endl;
	cout << "=========END==========" << endl;
	SudokuSolver sudoku_out;
	
	 sudoku_out.extractMatrix(Sudoku);

	if (SolveSudoku(sudoku_out.tab) == true)  
        printSu(sudoku_out.tab);
    else
        cout << "No solution exists";  
	
	return 0;
}