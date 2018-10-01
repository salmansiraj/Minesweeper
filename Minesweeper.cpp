//
//  rec03.cpp
//  recitation 3
//
//  Created by salman siraj on 9/20/18.
//  Copyright © 2018 salman siraj. All rights reserved.
//

#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Tile {
    bool visible;
    int counter = 0;
};

class Minesweeper {

private:
    const int board_size = 10;
    const int bomb_prob = 10;
    vector<vector<Tile>> board;
    
public:
    Minesweeper () {
        srand(time(NULL));
        newBoard();
        neighborCounters();
    }

    void display(bool display) const {
        // If true, display all counter attributes as well as bombs
        if (display == true) {
            for (int curr_row = 0; curr_row < board_size; curr_row++) {
                for (int curr_col = 0; curr_col < board_size; curr_col++) {
                    if (checkMine(curr_row, curr_col) == true) {
                        cout << "*" << " ";
                    }
                    else {
                            cout << board[curr_row][curr_col].counter << " ";
                        }
                }
                cout << endl;
            }
        }
        // If false, display board without showing the bombs and provide the counters to tiles that are visible
        else {
            for (int curr_row = 0; curr_row < board_size; curr_row++) {
                for (int curr_col = 0; curr_col < board_size; curr_col++) {
                    if (isVisible(curr_row, curr_col)) {
                        cout << board[curr_row][curr_col].counter << " ";
                    }
                    else {
                        cout << "- ";
                    }
                }
                cout << endl;
            }
        }
    }
    
    bool checkMine (int curr_row, int curr_col) const {
        return board[curr_row][curr_col].counter == -1;
    }
    
    bool isVisible (int curr_row, int curr_col) const {
        return board[curr_row][curr_col].visible;
    }
    
    bool validRow (int curr_row) {
        if (curr_row >= 0 && curr_row < board_size) {
            return true;
        }
        return false;
    }
    
    bool validCol (int curr_col) {
        if (curr_col >= 0 && curr_col < board_size) {
            return true;
        }
        return false;
    }
    
    bool done() const {
        // If an element contains -1(BOMB), AND is visible, then the game is over
        for (int curr_row = 0; curr_row < board_size; curr_row++) {
            for (int curr_col = 0; curr_col < board_size; curr_col++) {
                if (board[curr_row][curr_col].counter == -1) {
                    if (board[curr_row][curr_col].visible == true) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
    bool play (int row, int col) {
            // if there is a bomb, then end and return play
            if (board[row][col].counter == -1) {
                return false;
            }
            else if (board[row][col].counter > 0) {
                board[row][col].visible = true;
                return true;
            }
            else if (board[row][col].counter == 0) {
                    board[row][col].visible = true;
                    for (int i = -1; i < 2; i++) {
                        for (int j = -1; j < 2; j++) {
                            if (i == 0 && j == 0) {
                                continue;
                            }
                            else {
                                if (validRow(row + i) && validCol(col + j)) {
                                    if (isVisible(row + i, col + j) == false) {
                                        // Along with checking position validity
                                        //keep opening up all the nonvisible AND ones nearby who aren't near any bombs
                                        play(row + i, col + j);
                                    }
                                }
                            }
                        }
                    }
                }
        return true;
    }
    
    // Create a new board through building up a vector of tiled rows and randomizing a bomb for each tile
    void newBoard() {
        for (int row = 0; row < board_size; row++) {
            vector<Tile> rowVec;
            for (int col = 0; col < board_size; col++) {
                Tile currTile;
                currTile.visible = false;
                if ((rand() % 100) < bomb_prob) {
                    currTile.counter = -1;
                }
                rowVec.push_back(currTile);
            }
            board.push_back(rowVec);
        }
    }
    
    // For each tile on the board, we check the neighboring tiles for bombs increment the counter attribute for that specific tile
    void neighborCounters () {
        for (int curr_row = 0; curr_row < board_size; curr_row++) {
            for (int curr_col = 0; curr_col < board_size; curr_col++) {
                for (int i = -1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {
                        if (i == 0 && j == 0) {
                            continue;
                        }
                        else {
                            if ((validRow(curr_row + i) and validCol(curr_col + j)) and (board[curr_row + i][curr_col + j].counter == -1)) {
                                board[curr_row][curr_col].counter += 1;
                            }
                        }
                    }
                }
            }
        }
    }
};



int main() {
    Minesweeper sweeper;
//    sweeper.display(false);
//     Continue until only invisible cells are bombs
    while (!sweeper.done()) {
        sweeper.display(false); // display board without bombs

        int row_sel = -1, col_sel = -1;
        while (row_sel == -1) {
            // Get a valid move
            int r, c;
            cout << "row? ";
            cin >> r;
            if (!sweeper.validRow(r)) {
                cout << "Row out of bounds\n";
                continue;
            }
            cout << "col? ";
            cin >> c;
            if (!sweeper.validCol(c)) {
                cout << "Column out of bounds\n";
                continue;
            }
            if (sweeper.isVisible(r,c)) {
                cout << "Square already visible\n";
                continue;
            }
            row_sel = r;
            col_sel = c;
        }
//         Set selected square to be visible. May effect other cells.
        if (!sweeper.play(row_sel,col_sel)) {
            cout << "Sorry, you died..\n";
            sweeper.display(true); // Final board with bombs shown
            exit(0);
        }
    }
    // Ah! All invisible cells are bombs, so you won!
    cout << "You won!!!!\n";
    sweeper.display(true); // Final board with bombs shown
}

