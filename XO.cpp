#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class Board {
private:
    const int ROWS = 6;
    const int COLS = 7;
    vector<vector<char>> grid;

public:
    Board() : grid(ROWS, vector<char>(COLS, ' ')) {}

    void printBoard() const {
        cout << "\n";
        for (int i = 0; i < ROWS; i++) {
            cout << "|";
            for (int j = 0; j < COLS; j++) {
                cout << grid[i][j] << "|";
            }
            cout << "\n";
        }
        cout << "---------------\n";
        cout << " 1 2 3 4 5 6 7\n";
    }

    bool isValidMove(int col) const {
        return col >= 0 && col < COLS && grid[0][col] == ' ';
    }

    void makeMove(int col, char symbol) {
        for (int i = ROWS - 1; i >= 0; i--) {
            if (grid[i][col] == ' ') {
                grid[i][col] = symbol;
                break;
            }
        }
    }

    int numberOf4(char player) const {
        int s = 0;
        // Check horizontal
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS - 3; j++) {
                if (grid[i][j] == player &&
                    grid[i][j+1] == player &&
                    grid[i][j+2] == player &&
                    grid[i][j+3] == player) {
                    s++;
                }
            }
        }

        // Check vertical
        for (int i = 0; i < ROWS - 3; i++) {
            for (int j = 0; j < COLS; j++) {
                if (grid[i][j] == player &&
                    grid[i+1][j] == player &&
                    grid[i+2][j] == player &&
                    grid[i+3][j] == player) {
                    s++;
                }
            }
        }

        // Check diagonal (top-left to bottom-right)
        for (int i = 0; i < ROWS - 3; i++) {
            for (int j = 0; j < COLS - 3; j++) {
                if (grid[i][j] == player &&
                    grid[i+1][j+1] == player &&
                    grid[i+2][j+2] == player &&
                    grid[i+3][j+3] == player) {
                    s++;
                }
            }
        }

        // Check diagonal (top-right to bottom-left)
        for (int i = 0; i < ROWS - 3; i++) {
            for (int j = 3; j < COLS; j++) {
                if (grid[i][j] == player &&
                    grid[i+1][j-1] == player &&
                    grid[i+2][j-2] == player &&
                    grid[i+3][j-3] == player) {
                    s++;
                }
            }
        }

        return s;
    }

    int check4or3(char currentPlayer, int col) {
        int row;
        int output = -1;
        for (int i = ROWS-1; i>=0 ; i--) {
            if (grid[i][col] == ' ') {
                row = i;
                break;
            }
        }
        
        // Check horizontal
        if (col <= COLS - 4 && grid[row][col+1] == currentPlayer &&
        grid[row][col+2] == currentPlayer && grid[row][col+3] == currentPlayer) return 4;

        if (col >= 3 && grid[row][col-1] == currentPlayer &&
        grid[row][col-2] == currentPlayer && grid[row][col-3] == currentPlayer) return 4;

        else if (row <= ROWS-3) {
                if (grid[row+1][col] == currentPlayer &&
            grid[row+2][col] == currentPlayer)
            output = 3;
        }
        

        // Check vertical
        if (row <= ROWS-4) {
            if (grid[row+1][col] == currentPlayer &&
                grid[row+2][col] == currentPlayer &&
                grid[row+3][col] == currentPlayer) 
                return 4;
            
            else if (row <= ROWS-3) {
                if (grid[row+1][col] == currentPlayer &&
                    grid[row+2][col] == currentPlayer)
                    output = 3;
            }
        }

        // Check diagonal
        if (row <= ROWS-4 && col <= COLS-4) {
            if (grid[row+1][col+1] == currentPlayer &&
                grid[row+2][col+2] == currentPlayer &&
                grid[row+3][col+3] == currentPlayer)
                return 4;

            else if (row <= ROWS-3 && col <= COLS-3) {
                if (grid[row+1][col+1] == currentPlayer &&
                    grid[row+2][col+2] == currentPlayer) 
                    output= 3;
            }
        }

        // Check diagonal (top-right to bottom-left)
        if (row <= ROWS-4 && col >= 3) {
            if (grid[row+1][col-1] == currentPlayer &&
                grid[row+2][col-2] == currentPlayer &&
                grid[row+3][col-3] == currentPlayer) {
                return 4;
            }
            else if (row <= ROWS-3 && col >= 2) {
                if (grid[row+1][col-1] == currentPlayer &&
                    grid[row+2][col-2] == currentPlayer)
                    output= 3;}
        }

        return output;
    }
};

class Player {
private:
    string name;
    char symbol;

public:
    Player(string n, char s) : name(n), symbol(s) {}
    string getName() const { return name; }
};

class AI {
public:
int getMove(Board& board, char symbol, char opponent) {
    int col3 = -1,check;
    for (int col = 0; col < 7; col++) {
        if (board.isValidMove(col)) {
            check = board.check4or3(symbol, col);
            if (check == 4) return col;
            if (board.check4or3(opponent, col) == 4) return col;
            if (check == 3) col3 = col;
        }
}
    if (col3 != -1) return col3;
    else{
        srand(time(nullptr));
        int weighted_array[] = {0, 1, 2, 2, 3, 3, 3, 4, 4, 5, 6};
        while (1){
            int index = rand() % 11;
            if (board.isValidMove(weighted_array[index])) return weighted_array[index];
        }
    }
}
};

class Game {
private:
    Board board;
    Player player1;
    Player player2;
    AI computer;
    bool vsComputer;
    char currentPlayer;

public:
    Game() : player1("Player 1", 'X'), player2("Player 2", 'O'), vsComputer(false), currentPlayer('X') {}

    void selectMode() {
        int choice;
        cout << "Select game mode:\n1. Player vs Player\n2. Player vs Computer\nEnter choice: ";
        cin >> choice;
        vsComputer = (choice == 2);
    }

    void play() {
        srand(time(0)); // Initialize random seed
        int move;
        int moves = 0;
        int moveCount = 0;

        while(moveCount <= 41) {
            board.printBoard();  // showStatus
        
            char currentPlayerSymbol = (moveCount % 2 == 0) ? 'X' : 'O';
            bool isComputerTurn = vsComputer && (currentPlayerSymbol == 'O');

            if (isComputerTurn) {
                // Computer's turn
                computer.getMove(board, 'O', 'X');
                move = computer.getMove(board, 'O', 'X');
                cout << "Computer plays: " << move + 1 << endl;
                //cout << board.check4or3('O', 0) << endl;
            } else {
                // Human player's turn
                cout << "Player " << currentPlayerSymbol << ", enter column (1-7): ";
                cin >> move;
                move--;  // Convert to 0-based index
            }

            // Validate move
            if (!board.isValidMove(move)) {
                cout << "This column is full. Choose again.\n";
                continue;
            }

            // Make the move
            board.makeMove(move, currentPlayerSymbol);
            moveCount ++;
        }

    // Game over - determine winner
        board.printBoard();
        int X = board.numberOf4('X');
        int O = board.numberOf4('O');
        
        if (X > O) {
            cout << player1.getName() << " wins with " << X << " four-in-a-rows!" << endl;
        } else if (O > X) {
            if (vsComputer) {
                cout << "Computer wins with " << O << " four-in-a-rows!" << endl;
            } else {
                cout << player2.getName() << " wins with " << O << " four-in-a-rows!" << endl;
            }
        } else {
            cout << "X=O! Both have " << X << " four-in-a-rows." << endl;
        }
    }
};

int main() {
    Game game;
    game.selectMode();
    game.play();
    return 0;
}