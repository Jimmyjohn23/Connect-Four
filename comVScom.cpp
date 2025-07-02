#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

const int ROWS = 6;
const int COLS = 7;
const int CELL_SIZE = 100;

enum Player { None, Human, AI };

class GameBoard {
private:
    std::vector<std::vector<Player>> board;

public:
    GameBoard() : board(ROWS, std::vector<Player>(COLS, None)) {}

    void draw(sf::RenderWindow &window) const {
        window.clear(sf::Color::Blue);
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c < COLS; ++c) {
                sf::CircleShape piece(CELL_SIZE / 2 - 5);
                piece.setPosition(c * CELL_SIZE + 5, r * CELL_SIZE + 5);
                piece.setFillColor(sf::Color::White);
                Player p = board[r][c];
                if (p == Human) piece.setFillColor(sf::Color::Red);
                else if (p == AI) piece.setFillColor(sf::Color::Yellow);
                window.draw(piece);
            }
        }
        window.display();
    }

    bool isValidMove(int col) const {
        return col >= 0 && col < COLS && board[0][col] == None;
    }

    bool makeMove(int col, Player player) {
        if (!isValidMove(col)) return false;
        for (int r = ROWS - 1; r >= 0; --r) {
            if (board[r][col] == None) {
                board[r][col] = player;
                return true;
            }
        }
        return false;
    }

    void undoMove(int col) {
        for (int r = 0; r < ROWS; ++r) {
            if (board[r][col] != None) {
                board[r][col] = None;
                break;
            }
        }
    }

    bool isFull() const {
        for (int c = 0; c < COLS; ++c) {
            if (isValidMove(c)) return false;
        }
        return true;
    }

    bool checkWin(Player player) const {
        // Horizontal
        for (int r = 0; r < ROWS; ++r)
            for (int c = 0; c <= COLS - 4; ++c)
                if (board[r][c] == player && board[r][c+1] == player &&
                    board[r][c+2] == player && board[r][c+3] == player)
                    return true;
        // Vertical
        for (int c = 0; c < COLS; ++c)
            for (int r = 0; r <= ROWS - 4; ++r)
                if (board[r][c] == player && board[r+1][c] == player &&
                    board[r+2][c] == player && board[r+3][c] == player)
                    return true;
        // Diagonal down-right
        for (int r = 0; r <= ROWS - 4; ++r)
            for (int c = 0; c <= COLS - 4; ++c)
                if (board[r][c] == player && board[r+1][c+1] == player &&
                    board[r+2][c+2] == player && board[r+3][c+3] == player)
                    return true;
        // Diagonal up-right
        for (int r = 3; r < ROWS; ++r)
            for (int c = 0; c <= COLS - 4; ++c)
                if (board[r][c] == player && board[r-1][c+1] == player &&
                    board[r-2][c+2] == player && board[r-3][c+3] == player)
                    return true;
        return false;
    }

    int evaluate(Player aiPlayer, Player humanPlayer) const {
        if (checkWin(aiPlayer)) return 1000;
        if (checkWin(humanPlayer)) return -1000;
        return 0;
    }

    int minimax(int depth, bool maximizing, Player aiPlayer, Player humanPlayer, int alpha, int beta) {
        int score = evaluate(aiPlayer, humanPlayer);
        if (depth == 0 || score == 1000 || score == -1000 || isFull())
            return score;
        if (maximizing) {
            int maxEval = -100000;
            for (int c = 0; c < COLS; ++c) {
                if (!isValidMove(c)) continue;
                makeMove(c, aiPlayer);
                int eval = minimax(depth - 1, false, aiPlayer, humanPlayer, alpha, beta);
                undoMove(c);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) break;
            }
            return maxEval;
        } else {
            int minEval = 100000;
            for (int c = 0; c < COLS; ++c) {
                if (!isValidMove(c)) continue;
                makeMove(c, humanPlayer);
                int eval = minimax(depth - 1, true, aiPlayer, humanPlayer, alpha, beta);
                undoMove(c);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) break;
            }
            return minEval;
        }
    }

    int getAIMove(Player aiPlayer, Player humanPlayer) {
        int bestScore = -100000;
        int bestCol = 0;
        const int MAX_DEPTH = 5;
        for (int c = 0; c < COLS; ++c) {
            if (!isValidMove(c)) continue;
            makeMove(c, aiPlayer);
            int score = minimax(MAX_DEPTH - 1, false, aiPlayer, humanPlayer, -100000, 100000);
            undoMove(c);
            if (score > bestScore) {
                bestScore = score;
                bestCol = c;
            }
        }
        return bestCol;
    }
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Coin flip to decide who starts
    std::cout << "Guess heads or tails to start (type heads/tails): ";
    std::string guess;
    std::cin >> guess;
    std::transform(guess.begin(), guess.end(), guess.begin(), ::tolower);
    while (guess != "heads" && guess != "tails") {
        std::cout << "Please type 'heads' or 'tails': ";
        std::cin >> guess;
        std::transform(guess.begin(), guess.end(), guess.begin(), ::tolower);
    }
    int coinFlip = std::rand() % 2; // 0 heads, 1 tails
    int playerCoinFlip = (guess == "heads") ? 0 : 1;
    std::cout << "Coin flip result: " << (coinFlip == 0 ? "heads" : "tails") << std::endl;

    Player currentPlayer = (playerCoinFlip != coinFlip) ? AI : Human;
    std::cout << (currentPlayer == Human ? "You go first." : "AI goes first.") << std::endl;

    GameBoard game;
    sf::RenderWindow window(sf::VideoMode(COLS * CELL_SIZE, ROWS * CELL_SIZE), "Connect Four");
    bool gameOver = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!gameOver && event.type == sf::Event::MouseButtonPressed && currentPlayer == Human) {
                int col = game.getAIMove(Human, AI);

                if (game.makeMove(col, Human)) {
                    if (game.checkWin(Human)) {
                        std::cout << "You win!" << std::endl;
                        gameOver = true;
                    } else {
                        currentPlayer = AI;
                    }
                }
            }
        }

        if (!gameOver && currentPlayer == AI) {
            int col = game.getAIMove(AI, Human);
            game.makeMove(col, AI);
            if (game.checkWin(AI)) {
                std::cout << "AI wins!" << std::endl;
                gameOver = true;
            } else {
                currentPlayer = Human;
            }
        }

        game.draw(window);
    }

    return 0;
}