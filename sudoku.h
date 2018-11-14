#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <random>
#include <ctime>
#include <vector>

using std::vector;

const int GRID_SIZE = 9;

class Sudoku {
    public:
        Sudoku();
        ~Sudoku() = default;

        void generate();
        void solve();
        void draw();    
        void drawSolution();

        void debug_copy_solution();
        void debug_is_solution_right();
    private:
        bool n_in_quad(int n, int i, int j);
        bool n_in_col(int n, int j);

        vector<bool> get_quad_possibles(int i, int j);
        vector<bool> get_col_possibles(int j);
        vector<bool> get_row_possibles(int i);

        void set_cell_solved(int i, int j, int n);
        void check_solved_cells(int i, int j);

        int grid[GRID_SIZE][GRID_SIZE];
        int solution[GRID_SIZE][GRID_SIZE];
        vector<bool> possibles[GRID_SIZE][GRID_SIZE];

        int solution_changes;
};

#endif // SUDOKU_H