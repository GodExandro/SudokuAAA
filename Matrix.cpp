#include "Matrix.h"

Matrix::Matrix()
{

}

Matrix::Matrix(unsigned rowsNo, unsigned colsNo) : rows(rowsNo) {
	for (auto& row : rows) {
		row.resize(colsNo);
	}
}

int& Matrix::at(unsigned rowIndex, unsigned colIndex) {
	return rows.at(rowIndex).at(colIndex);
}

const int& Matrix::at(unsigned rowIndex, unsigned colIndex) const {
	return rows.at(rowIndex).at(colIndex);
}

unsigned Matrix::getRowsNo() const {
	return rows.size();
}

unsigned Matrix::getColsNo() const {
	if (rows.size()) {
		return rows[0].size();
	}

	return 0;
}