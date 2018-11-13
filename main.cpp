#include <iostream>
#include "sudoku.h"

int main() {
    Sudoku sudoku;
    sudoku.generate();
    sudoku.draw();
    return 0;
}