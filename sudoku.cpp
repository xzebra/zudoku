#include "sudoku.h"

typedef std::mt19937 RandGenerator;
RandGenerator rng;

Sudoku::Sudoku() {
    // Clear grid memory
    for(int i = 0; i < GRID_SIZE; ++i)
        for(int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = 0;
            solution[i][j] = 0;
        }
    // Populate random number generator seed
    rng.seed(std::time(nullptr));
}

bool Sudoku::n_in_quad(int n, int i, int j) {
    // translate quad coords to center position in grid
    int center[2] = {i*3 + 1, j*3 + 1};
    // iterate over quad to check if n exists in it
    for(int x = center[0]-1; x <= center[0]+1; ++x) {
        for(int y = center[1]-1; y <= center[1]+1; ++y) {
            if(solution[x][y] == 0) return false;
            if(n == solution[x][y]) return true;
        }
    }
    return false;
}

bool Sudoku::n_in_col(int n, int j) {
    // iterate over the col to check if n exists in it
    for(int i = 0; i < GRID_SIZE; ++i) {
        if(solution[i][j] == 0) break;
        if(solution[i][j] == n) return true;
    }
    return false;
}

vector<bool> Sudoku::get_quad_possibles(int i, int j) {
    vector<bool> output(9, true);
    // translate quad coords to center position in grid
    int center[2] = {i*3 + 1, j*3 + 1};
    // iterate over quad to check if n exists in it
    for(int x = center[0]-1; x <= center[0]+1; ++x) {
        for(int y = center[1]-1; y <= center[1]+1; ++y) {
            if(this->grid[x][y] != 0)
                output[this->grid[x][y]-1] = false;
        }
    }
    return output;
}

vector<bool> Sudoku::get_col_possibles(int j) {
    vector<bool> output(9, true);
    for(int i = 0; i < GRID_SIZE; ++i) {
        if(this->grid[i][j] != 0)
            output[this->grid[i][j]-1] = false;
    }
    return output;
}

vector<bool> Sudoku::get_row_possibles(int i) {
    vector<bool> output(9, true);
    for(int j = 0; j < GRID_SIZE; ++j) {
        if(this->grid[i][j] != 0)
            output[this->grid[i][j]-1] = false;
    }
    return output;
}

int possiblesCount(vector<bool> possibles) {
    int count = 0;
    for(int i = 0; i < possibles.size(); ++i) {
        if(possibles[i]) ++count;
    }
    return count;
}

void Sudoku::generate() {
    vector<bool> oneToNine(9, true);

    for(int i = 0; i < GRID_SIZE; ++i) {
        // Generate horizontal possibles
        vector<bool> h_possibles = oneToNine;

        int j = 0;
        while(j < GRID_SIZE) {          
            // If not initialized yet store the possibles of the row in that pos
            if(this->possibles[i][j].size() == 0) {
                this->possibles[i][j] = h_possibles;             
            }

            // Check if we are out of possible numbers
            if(possiblesCount(this->possibles[i][j]) == 0) {   
                // Replenish this square numbers
                this->possibles[i][j].resize(0);

                --j; // Go back 1 square
                // Remove the chosen one from the possibles
                this->possibles[i][j][solution[i][j]-1] = false;
                h_possibles[solution[i][j]-1] = true;
                solution[i][j] = 0;
            } else {
                // Get a number randomly from the available numbers
                std::uniform_int_distribution<uint32_t> tempGenerator(0, possiblesCount(this->possibles[i][j])-1);                
                int n = tempGenerator(rng)+1;
                int choosen = 0;
                while(true) { // Choose from the possible ones the n-one
                    if(this->possibles[i][j][choosen]) --n;
                    if(n == 0) break;
                    ++choosen;
                }
                

                // Check if it conflicts
                if(n_in_quad(choosen+1, i/3, j/3) || n_in_col(choosen+1, j)) {
                    // Remove from available numbers for this square
                    this->possibles[i][j][choosen] = false;
                } else {
                    // Use it and go forward
                    solution[i][j] = choosen+1;
                    h_possibles[choosen] = false;
                    ++j;        
                }             
            }
        }
    }
}

// ---------------------------------
// ------- Solving functions -------
// ---------------------------------

// n is 0-based
void Sudoku::set_cell_solved(int i, int j, int n) {
    int center[2] = {(i/3)*3 + 1, (j/3)*3 + 1};
    // iterate over quad to remove n from possibles
    for(int x = center[0]-1; x <= center[0]+1; ++x) {
        for(int y = center[1]-1; y <= center[1]+1; ++y) {
            this->possibles[x][y][n] = false;
        }
    }

    // Vertical and horizontal removal
    for(int x = 0; x < GRID_SIZE; ++x) {
        this->possibles[x][j][n] = false;
        this->possibles[i][x][n] = false;
    }

    this->solution_changes++;
    this->grid[i][j] = n+1;
}

void Sudoku::check_solved_cells(int i, int j) {
    if(possiblesCount(this->possibles[i][j]) == 1) {
        // Get the only possible one
        for(int x = 0; x < 9; ++x) {
            if(this->possibles[i][j][x]) {
                set_cell_solved(i, j, x);
                break;
            }
        }             
    }
}

void Sudoku::solve() {
    this->solution_changes = 0;
    // clear memory from possibles array
    for(int i = 0; i < GRID_SIZE; ++i) {
        for(int j = 0; j < GRID_SIZE; ++j) {
            this->possibles[i][j] = vector<bool>(9, false);
        }
    }

    while(true) {
        int lastChanges = this->solution_changes;
        // Get absolute vertical possibilities
        vector<bool> v_possibles[GRID_SIZE];
        vector<bool> h_possibles[GRID_SIZE];
        for(int j = 0; j < GRID_SIZE; ++j) {
            v_possibles[j] = get_col_possibles(j);         
        }

        // Calculate all possibles of each cell
        for(int i = 0; i < GRID_SIZE; ++i) {
            h_possibles[i] = get_row_possibles(i);
            for(int j = 0; j < GRID_SIZE; ++j) {
                vector<bool> q_possibles = get_quad_possibles(i/3, j/3);
                for(int n = 0; n < 9; ++n) {
                    this->possibles[i][j][n] = (h_possibles[i][n] && v_possibles[j][n] && q_possibles[n]);
                }
            }
        }

        // Check for solved cells
        for(int i = 0; i < GRID_SIZE; ++i) {
            for(int j = 0; j < GRID_SIZE; ++j) {
                if(this->grid[i][j] == 0) {
                    check_solved_cells(i, j);
                    // (Last Remaining Cell in a Box)
                }
            }
        }

        // Avoid infinite loops just for debugging
        if(lastChanges == this->solution_changes) break;
    }
}

void Sudoku::debug_copy_solution() {
    for(int i = 0; i < GRID_SIZE; ++i) {
        for(int j = 0; j < GRID_SIZE; ++j) {
            this->grid[i][j] = this->solution[i][j];
        }
    }
    
    std::uniform_int_distribution<uint32_t> tempGenerator(0, 8);
    for(int i = 0; i < 50; ++i) {
        this->grid[tempGenerator(rng)][tempGenerator(rng)] = 0;
    }
}

void Sudoku::debug_is_solution_right() {
    for(int i = 0; i < GRID_SIZE; ++i) {
        for(int j = 0; j < GRID_SIZE; ++j) {
            if(this->grid[i][j] != this->solution[i][j]) {
                std::cout << "Solution is not right" << std::endl;
                return;
            }
        }
    }
    std::cout << "Solution is right" << std::endl;
}

// ---------------------------------
// ------- Drawing functions -------
// ---------------------------------
void draw_separator() { 
    std::cout << "+-------+-------+-------+" << std::endl; 
}

char draw_cell_content(const int n) {
    // if cell holds a 0 (undefined number), draw a '.' instead
    if(n == 0) return '.';
    else return n + '0';
}

void drawSudoku(int grid[][GRID_SIZE]) {
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

void Sudoku::draw() {
    drawSudoku(this->grid);
}

void Sudoku::drawSolution() {
    drawSudoku(this->solution);
}