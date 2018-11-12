#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>

const int GRID_SIZE = 9;

class Sudoku {
    public:
        Sudoku();
        ~Sudoku() = default;

        void generate();
        void draw();
    private:
        int grid[GRID_SIZE][GRID_SIZE];
        int solution[GRID_SIZE][GRID_SIZE];
};

#endif // SUDOKU_H