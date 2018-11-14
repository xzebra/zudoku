#include <iostream>
#include "sudoku.h"

int main() {
    Sudoku sudoku;
    sudoku.generate();
    sudoku.debug_copy_solution();
    sudoku.draw();
    sudoku.solve();
    sudoku.draw();
    sudoku.debug_is_solution_right();
    return 0;
}