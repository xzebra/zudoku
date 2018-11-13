#include "sudoku.h"

typedef std::mt19937 RandGenerator;
RandGenerator rng;

Sudoku::Sudoku() {
    // Clear grid memory
    for(int i = 0; i < GRID_SIZE; ++i)
        for(int j = 0; j < GRID_SIZE; ++j) 
            grid[i][j] = 0;
    // Populate random number generator seed
    rng.seed(std::time(nullptr));
}

void generate_random_row(int row[]) {
    // create a distribution to generate integers in range [0,8]
    std::uniform_int_distribution<uint32_t> genNum(0,8);
    
    // Generate increasing row
    for(int j = 0; j < GRID_SIZE; ++j) {
        row[j] = j+1;
    }
    // Random swaps
    for(int j = 0; j < GRID_SIZE; ++j) {
        std::swap(row[genNum(rng)], row[genNum(rng)]);
    }
}

bool Sudoku::n_in_quad(int n, int i, int j) {
    // translate quad coords to center position in grid
    int center[2] = {i*3 + 1, j*3 + 1};
    // iterate over quad to check if n exists in it
    for(int x = center[0]-1; x <= center[0]+1; ++x) {
        for(int y = center[1]-1; y <= center[1]+1; ++y) {
            if(grid[x][y] == 0) return false;
            if(n == grid[x][y]) return true;
        }
    }
    return false;
}

bool Sudoku::n_in_col(int n, int j) {
    // iterate over the col to check if n exists in it
    for(int i = 0; i < GRID_SIZE; ++i) {
        if(grid[i][j] == 0) break;
        if(grid[i][j] == n) return true;
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
            if(grid[x][y] == 0)  return output;
            output[grid[x][y]] = false;
        }
    }
    return output;
}

vector<bool> Sudoku::get_col_possibles(int j) {
    vector<bool> output(9, true);
    for(int i = 0; i < GRID_SIZE; ++i) {
        if(grid[i][j] == 0) return output;
        output[grid[i][j]] = false;
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
    //generate_random_row(grid[0]);

    vector<bool> oneToNine(9, true);
    vector<bool> possibles[GRID_SIZE][GRID_SIZE];

    for(int i = 0; i < GRID_SIZE; ++i) {
        // Generate horizontal possibles
        vector<bool> h_possibles = oneToNine;

        int j = 0;
        while(j < GRID_SIZE) {          
            // If not initialized yet store the possibles of the row in that pos
            if(possibles[i][j].size() == 0) {
                possibles[i][j] = h_possibles;             
            }

            // Check if we are out of possible numbers
            if(possiblesCount(possibles[i][j]) == 0) {   
                // Replenish this square numbers
                possibles[i][j].resize(0);

                --j; // Go back 1 square
                // Remove the chosen one from the possibles
                possibles[i][j][grid[i][j] - 1] = false;
                h_possibles[grid[i][j]-1] = true;
                grid[i][j] = 0;
            } else {
                // Get a number randomly from the available numbers
                std::uniform_int_distribution<uint32_t> tempGenerator(0, possiblesCount(possibles[i][j])-1);                
                int n = tempGenerator(rng)+1;
                int choosen = 0;
                while(true) { // Choose from the possible ones the n-one
                    if(possibles[i][j][choosen]) --n;
                    if(n == 0) break;
                    ++choosen;
                }
                

                // Check if it conflicts
                if(n_in_quad(choosen+1, i/3, j/3) || n_in_col(choosen+1, j)) {
                    // Remove from available numbers for this square
                    possibles[i][j][choosen] = false;
                } else {
                    // Use it and go forward
                    grid[i][j] = choosen+1;
                    h_possibles[choosen] = false;
                    ++j;        
                }             
            }
        }
    }
}

void draw_separator() { 
    std::cout << "+-------+-------+-------+" << std::endl; 
}

char draw_cell_content(const int n) {
    // if cell holds a 0 (undefined number), draw a '.' instead
    if(n == 0) return '.';
    else return n + '0';
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