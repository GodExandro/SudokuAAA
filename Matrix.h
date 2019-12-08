#pragma once

#include <vector>

class Matrix {
public:
	Matrix();
	Matrix(unsigned rowsNo, unsigned colsNo);

	int& at(unsigned rowIndex, unsigned colIndex);
	const int& at(unsigned rowIndex, unsigned colIndex) const;

	unsigned getRowsNo() const;
	unsigned getColsNo() const;

private:
	std::vector<std::vector<int>> rows;
};