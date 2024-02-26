#include <iostream>
#include <string>
#include "spielfeld.hh"


// For saving the best moves
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Define player type
#define HUMAN_PLAYER GameBoard::Player_O
#define AI_PLAYER GameBoard::Player_X

// Define  Result Score Values for MiniMax Algorithm
#define PLAYER_X_WIN 1000
#define PLAYER_O_WIN -1000

// Struct for player statistics
struct PlayerStatistics {
    float playerXWins;
    float playerOWins;
};

// Parse user input and return if the input is valid
bool parseField(string input, int& row, int& column);

GameBoard::Player switchCurrentPlayer(GameBoard::Player player);

// MiniMax Algorithm Functions
int miniMax(GameBoard board, bool isMaximizing);
void makeBestMove(GameBoard& board, GameBoard::Player& currentPlayer);

// Game Loop Functions
void inGameLoop(GameBoard& board, GameBoard::Player& currentPlayer, GameBoard::Scoring& gameState, PlayerStatistics& playerStats);
void playHumanTurn(GameBoard& board, GameBoard::Player& currentPlayer);
void playComputerTurn(GameBoard& board, GameBoard::Player& currentPlayer);
void handleGameResult(GameBoard::Scoring& gameState, GameBoard::Player& currentPlayer,PlayerStatistics& playerStats, GameBoard& board);


void printGameStatistics(GameBoard& board, PlayerStatistics& playerStats);

int main() {
    // Seed random number generator
    srand(time(0));
    // Initialize game board and player statistics
    GameBoard board;
    GameBoard::Player currentPlayer = GameBoard::Player_X;
    GameBoard::Scoring gameState;
    PlayerStatistics playerStats = {.playerXWins = 0, .playerOWins = 0};

    bool continuePlaying;
    do {
        inGameLoop(board, currentPlayer, gameState, playerStats);
        cout << "Play again? (y/n)" << endl;
        string input;
        getline(cin, input);
        continuePlaying = (input == "y");
        board = GameBoard(); // reset game board
    } while (continuePlaying);

    return 0;
}

// Parse user input and return if the input is valid
bool parseField(string input, int& row, int& column) {

    // To handle input like "A 3" or "A3"
    if ((input.length() == 3 && input[1] == ' ') || input.length() == 2) {
        // Remove space if input is "A 3"
        if (input.length() == 3) {
            input[1] = input[2];
            input[2] = '\0';
        }

        // Set row and column as int based on user input
        for (int i = 0; i < input.length(); i++) {
            switch (input[i]) {
                case 'A':
                case 'B':
                case 'C':
                    column = input[i] - 'A';
                    break;
                case 'a':
                case 'b':
                case 'c':
                    column = input[i] - 'a';
                    break;
                case '1':
                case '2':
                case '3':
                    row = input[i] - '1';
                    break;
            }
        }
        return true;
    }
    return false;
}

GameBoard::Player switchCurrentPlayer(GameBoard::Player player) {
    return (player == GameBoard::Player_X) ? GameBoard::Player_O : GameBoard::Player_X;
}

void printGameStatistics(GameBoard& board, PlayerStatistics& playerStats) {
    // For better readability
    float totalGames = playerStats.playerXWins + playerStats.playerOWins;
    float winRateX = (playerStats.playerXWins / totalGames) * 100;

    cout << "--------------------------" << endl;
    cout << "Game Statistics:" << endl;
    cout << "X: " << playerStats.playerXWins << " wins" << endl;
    cout << "O: " << playerStats.playerOWins << " wins" << endl;
    cout << "Player X won " << winRateX << "% of the games." << endl;
    cout << "Player O won " << 100 - winRateX << "% of the games." << endl;
}

// MiniMax Algorithm Functions
int miniMax(GameBoard board, bool isMaximizing) {
    // Early return if the game is over
    switch (board.gameScore()) {
        case GameBoard::WinO:
            return PLAYER_O_WIN;
        case GameBoard::WinX:
            return PLAYER_X_WIN;
        case GameBoard::Draw:
            return 0;
        case GameBoard::Open:
            break;
    }

    if (isMaximizing) {
        // Maximizing Player
        int bestScore = INT_MIN;
        for (int row = 0; row < 3; row++) {
            for (int column = 0; column < 3; column++) {
                if (board.fieldIsEmpty(row, column)) {
                    board.setField(row, column, AI_PLAYER);
                    int score = miniMax(board, false);
                    board.resetField(row, column);
                    bestScore = max(score, bestScore);
                }
            }
        }
        return bestScore;
    } else {
        // Minimizing Player
        int bestScore = INT_MAX;
        for (int row = 0; row < 3; row++) {
            for (int column = 0; column < 3; column++) {
                if (board.fieldIsEmpty(row, column)) {
                    board.setField(row, column, HUMAN_PLAYER);
                    int score = miniMax(board, true);
                    board.resetField(row, column);
                    bestScore = min(score, bestScore);
                }
            }
        }
        return bestScore;
    }
}

// Make best move for AI with MiniMax Algorithm and a random move if multiple best moves are found
void makeBestMove(GameBoard& board, GameBoard::Player& currentPlayer) {
    int bestScore = INT_MIN;

    // Struct to store the best moves
    struct Move {
        int row;
        int column;
    };

    // Initialize the bestMoves vector as a way to dynamically store the best moves
    vector<Move> bestMoves;

    // Iterate through the board and store the best moves in the bestMoves array
    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 3; column++) {
            if (board.fieldIsEmpty(row, column)) {

                // Set the field to the current player and run the miniMax algorithm, e.g. go deeper into the game tree
                board.setField(row, column, currentPlayer);
                int score = miniMax(board, false);
                board.resetField(row, column);

                // Clear the vector if a better move is found
                if (score > bestScore) {
                    bestScore = score;
                    bestMoves.clear();
                }

                // Append the best move to the bestMoves vector
                if (score == bestScore) {
                    bestMoves.push_back({row, column});
                }
            }
        }
    }

    // Select a random move from the bestMoves vector
    if (!bestMoves.empty()) {
        int randomIndex = rand() % bestMoves.size();
        board.setField(bestMoves[randomIndex].row, bestMoves[randomIndex].column, currentPlayer);
    }
}

void inGameLoop(GameBoard& board, GameBoard::Player& currentPlayer, GameBoard::Scoring& gameState, PlayerStatistics& playerStats) {
    // Repeat until the game is over
    while ((gameState = board.gameScore()) == GameBoard::Open) {
        board.printBoard(cout);
        cout << currentPlayer << " is playing!" << endl;
        if (currentPlayer == HUMAN_PLAYER) {
            playHumanTurn(board, currentPlayer);
        } else {
            playComputerTurn(board, currentPlayer);
        }
    }
    handleGameResult(gameState, currentPlayer, playerStats, board);
}

void playHumanTurn(GameBoard& board, GameBoard::Player& currentPlayer) {
    cout << "Choose a field! (e.g., A3)" << endl;
    string input;
    getline(cin, input);
    int column, row;
    if (parseField(input, row, column)) {
        if (board.fieldIsEmpty(row, column)) {
            board.setField(row, column, currentPlayer);
            currentPlayer = switchCurrentPlayer(currentPlayer);
        } else {
            cout << "This field is already taken!" << endl;
            cout << "Try again!" << endl;
        }
    } else {
        cout << "A field consists of a letter (A-C) followed by a digit (1-3)!" << endl;
        cout << "Try again!" << endl;
    }
}

void playComputerTurn(GameBoard& board, GameBoard::Player& currentPlayer) {
    makeBestMove(board, currentPlayer);
    currentPlayer = switchCurrentPlayer(currentPlayer);
}

void handleGameResult(GameBoard::Scoring& gameState, GameBoard::Player& currentPlayer, PlayerStatistics& playerStats, GameBoard& board) {
    switch (gameState) {
        case GameBoard::WinO:
            cout << "Player O wins!" << endl;
            playerStats.playerOWins++;
            currentPlayer = switchCurrentPlayer(currentPlayer);
            printGameStatistics(board, playerStats);
            break;
        case GameBoard::WinX:
            cout << "Player X wins!" << endl;
            playerStats.playerXWins++;
            currentPlayer = switchCurrentPlayer(currentPlayer);
            printGameStatistics(board, playerStats);
            break;
        case GameBoard::Draw:
            cout << "Draw!" << endl;
            playerStats.playerOWins += 0.5;
            playerStats.playerXWins += 0.5;
            printGameStatistics(board, playerStats);
            break;
        default:
            break;
    }
}
