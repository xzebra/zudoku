#include "sudoku.h"

Sudoku::Sudoku() {
    // Clear grid memory
    for(int i = 0; i < GRID_SIZE; ++i)
        for(int j = 0; j < GRID_SIZE; ++j) 
            grid[i][j] = 0;
}

void Sudoku::generate() {}

void draw_separator() { 
    std::cout << "+-------+-------+-------+" << std::endl; 
}

char draw_cell_content(const int n) {
    // if cell holds a 0 (undefined number), draw a '.' instead
    if(n == 0) return '.';
    else return n - '0';
}

void Sudoku::draw() {
    for(int i = 0; i < GRID_SIZE; ++i) {
        if(i % 3 == 0) draw_separator();
        for(int j = 0; j < GRID_SIZE; ++j) {
            // draw col separator
            if (j % 3 == 0) std::cout << (!j ? "" : " ") << "|";
            std::cout << " " << draw_cell_content(grid[i][j]);
        }
        std::cout << " |" << std::endl;
    }
    draw_separator();
}