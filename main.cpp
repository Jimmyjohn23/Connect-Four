#include <vector>
#include <string>
#include <iostream>

class GameBoard {
private:
    static const int ROWS = 6;
    static const int COLS = 7;
    std::vector<std::vector<char>> board;

public:
    GameBoard() : board(ROWS, std::vector<char>(COLS, ' ')) {}

    void displayBoard() {
        std::cout << "\033[2J\033[H";
        for (const auto& row : board) {
            for (const auto& cell : row)
                std::cout << (cell == ' ' ? '.' : cell) << "\t";
            std::cout << std::endl;
        }
        std::cout << "-------------------------------------------------\n";
        for (int i = 0; i < COLS; ++i)
            std::cout << i << "\t";
        std::cout << "\n\n";
    }

    void makeMove(int col, char player) {
        if (col < 0 || col >= COLS) {
            std::cout << "Invalid column. Choose between 0 and " << COLS - 1 << ".\n";
            return;
        }

        for (int row = ROWS - 1; row >= 0; --row) {
            if (board[row][col] == ' ') {
                board[row][col] = player;
                return;
            }
        }
        std::cout << "Column " << col << " is full. Choose another column.\n";
    }

    bool checkWin(char player) {
        // Horizontal check
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col <= COLS - 4; ++col) {
                if (board[row][col] == player &&
                    board[row][col + 1] == player &&
                    board[row][col + 2] == player &&
                    board[row][col + 3] == player)
                    return true;
            }
        }

        // Vertical check
        for (int col = 0; col < COLS; ++col) {
            for (int row = 0; row <= ROWS - 4; ++row) {
                if (board[row][col] == player &&
                    board[row + 1][col] == player &&
                    board[row + 2][col] == player &&
                    board[row + 3][col] == player)
                    return true;
            }
        }

        // Diagonal down-right
        for (int row = 0; row <= ROWS - 4; ++row) {
            for (int col = 0; col <= COLS - 4; ++col) {
                if (board[row][col] == player &&
                    board[row + 1][col + 1] == player &&
                    board[row + 2][col + 2] == player &&
                    board[row + 3][col + 3] == player)
                    return true;
            }
        }

        // Diagonal up-right
        for (int row = 3; row < ROWS; ++row) {
            for (int col = 0; col <= COLS - 4; ++col) {
                if (board[row][col] == player &&
                    board[row - 1][col + 1] == player &&
                    board[row - 2][col + 2] == player &&
                    board[row - 3][col + 3] == player)
                    return true;
            }
        }

        return false;
    }
};

int main() {
    GameBoard gameBoard;
    gameBoard.displayBoard();
    while (true) {
        int col;
        char player = 'X'; // Player X starts
        std::cout << "Player " << player << ", enter column (0-6) to drop your piece: ";
        std::cin >> col;

        gameBoard.makeMove(col, player);
        gameBoard.displayBoard();

        if (gameBoard.checkWin(player)) {
            std::cout << "Player " << player << " wins!\n";
            break;
        }

        // Switch player
        char nextPlayer = 'O';
        std::cout << "Player " << nextPlayer << ", enter column (0-6) to drop your piece: ";
        std::cin >> col;
        gameBoard.makeMove(col, nextPlayer);
        gameBoard.displayBoard();
        if (gameBoard.checkWin(nextPlayer)) {
            std::cout << "Player " << nextPlayer << " wins!\n";
            break;
        }
    }
    return 0;
}
