cmake_minimum_required(VERSION 3.3.1)

project(Sudoku)

find_package(OpenCV REQUIRED)

#find_package( Tesseract REQUIRED )

include_directories(
	${OpenCV_INCLUDE_DIRS}
	${Tesseract_INCLUDE_DIRS}
)

add_executable(Sudoku
	main.cpp
	ImageProcessing.h
	ImageProcessing.cpp
	KNN.h
	KNN.cpp
	Matrix.h
	Matrix.cpp
	SudokuNumbers.h
	SudokuNumbers.cpp
	SolverAlgorithm.cpp
	SudokuSolver.h
	SudokuSolver.cpp
)

target_link_libraries(Sudoku
	${OpenCV_LIBS}
#	${Tesseract_LIBRARIES}
)
