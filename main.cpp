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
    bool isValidMove(int col) {
        return board[0][col] == ' ';
    }
    void undoMove(int col) {
        for (int row = 0; row < ROWS; ++row) {
            if (board[row][col] != ' ') {
                board[row][col] = ' ';
                break;
            }
        }
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
    int findWinningMove(char player) {
        for (int col = 0; col < COLS; ++col) {
            if (!isValidMove(col)) continue;
            makeMove(col, player);
            if (checkWin(player)) {
                undoMove(col);
                return col;
            }
            undoMove(col);
        }
        return -1; // No winning move found
    }
    int getAIMove(char aiPlayer, char humanPlayer) {
        // 1. Can AI win immediately?
        int winningMove = findWinningMove(aiPlayer);
        if (winningMove != -1)
            return winningMove;

        // 2. Can human win next? Block them
        int blockMove = findWinningMove(humanPlayer);
        if (blockMove != -1)
            return blockMove;

        // 3. Take center if available
        if (isValidMove(COLS / 2))
            return COLS / 2;

        // 4. Pick first valid move
        for (int col = 0; col < COLS; ++col)
            if (isValidMove(col))
                return col;

        return -1; // No valid moves, board full (should not happen if called properly)
    }
};

int main() {
    GameBoard gameBoard;
    gameBoard.displayBoard();
    char humanPlayer = 'X';
    char aiPlayer = 'O';

    while (true) {
        int col;

        // Human move
        std::cout << "Player " << humanPlayer << ", enter column (0-6) to drop your piece: ";
        std::cin >> col;
        while (!gameBoard.isValidMove(col)) {
            std::cout << "Invalid move. Try again: ";
            std::cin >> col;
        }
        gameBoard.makeMove(col, humanPlayer);
        gameBoard.displayBoard();
        if (gameBoard.checkWin(humanPlayer)) {
            std::cout << "Player " << humanPlayer << " wins!\n";
            break;
        }

        // AI move
        col = gameBoard.getAIMove(aiPlayer, humanPlayer);
        std::cout << "AI chooses column: " << col << "\n";
        gameBoard.makeMove(col, aiPlayer);
        gameBoard.displayBoard();
        if (gameBoard.checkWin(aiPlayer)) {
            std::cout << "AI wins!\n";
            break;
        }
    }
    return 0;
}
