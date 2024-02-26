#include "spielfeld.hh"
#include <iomanip> /* für setw() */

/* Macht das Tippen leichter */
using namespace std;

GameBoard::GameBoard() {
    /* Initialisiere GameBoard */
    for (int columns = 0; columns < 3; columns++) {
        for (int rows = 0; rows < 3; rows++) {
            position[rows][columns] = Leer;
        }
    }
}

void GameBoard::printBoard(ostream &os) {
    /* Spaltenkopf */
    os << "  ABC" << endl;
    for (int rows = 0; rows < 3; rows++) {
        /* Zeilenkopf */
        os << setw(1) << (rows + 1) << ' ';
        /* Zeileninhalt */
        for (int columns = 0; columns < 3; columns++) {
            os << position[rows][columns];
        }
        os << endl;
    }
}

void GameBoard::setField(int rows, int columns, Player player) {
    if (0 <= rows && rows <= 2 && 0 <= columns && columns <= 2) {
        position[rows][columns] = (Marking)player;
    }
}

void GameBoard::resetField(int rows, int columns) {
    if (0 <= rows && rows <= 2 && 0 <= columns && columns <= 2) {
        position[rows][columns] = Leer;
    }
}

GameBoard::Scoring GameBoard::gameScore() {
    if (hasWon(Player_O)) {
        return WinO;
    } else if (hasWon(Player_X)) {
        return WinX;
    } else if (allFieldsTaken()) {
        return Draw;
    } else {
        return Open;
    }
}

bool GameBoard::hasWon(Player player) {
    // Zeile belegt?
    for (int rows = 0; rows < 3; rows++) {
        if (position[rows][0] == (Marking)player &&
            position[rows][1] == (Marking)player &&
            position[rows][2] == (Marking)player) {
            return true;
        }
    }
    // Spalte belegt?
    for (int columns = 0; columns < 3; columns++) {
        if (position[0][columns] == (Marking)player &&
            position[1][columns] == (Marking)player &&
            position[2][columns] == (Marking)player) {
            return true;
        }
    }
    // 1. Hauptdiagonale belegt?
    if (position[0][0] == (Marking)player &&
        position[1][1] == (Marking)player &&
        position[2][2] == (Marking)player) {
        return true;
    }
    // 2. Hauptdiagonale belegt?
    if (position[0][2] == (Marking)player &&
        position[1][1] == (Marking)player &&
        position[2][0] == (Marking)player) {
        return true;
    }
    return false;
}
bool GameBoard::winPossible() {
    // TODO:
    /*
     * 1. Prüfe, ob ein Spieler gewonnen hat
     * 2. Prüfe, ob alle Felder belegt sind
     * 3. Prüfe, ob es noch gewinnmoeglichkeiten gibt
     * 3.1. Prüfe, ob ein Spieler in der nächsten Runde gewinnen kann
     * 3.2. Prüfe, ob ein Spieler in der nächsten Runde verlieren kann
     * 3.3. Prüfe, ob ein Spieler in der nächsten Runde ein Feld belegen kann, dass den Gegner daran hindert zu gewinnen
     * 3.4. Prüfe, ob ein Spieler in der nächsten Runde ein Feld belegen kann, dass den Gegner daran hindert zu verlieren
     */
    if (hasWon(Player_O) || hasWon(Player_X) || allFieldsTaken()) {
        return false;
    }else{}
}
void checkNextBestMove(GameBoard board, GameBoard::Player player) {

}

bool GameBoard::allFieldsTaken() {
    for (int columns = 0; columns < 3; columns++) {
        for (int rows = 0; rows < 3; rows++) {
            if (position[rows][columns] == Leer) {
                return false;
            }
        }
    }
    return true;
}

/** Überladener <<-Operator für GameBoard-Markierungen */
std::ostream& operator<<(std::ostream& os, GameBoard::Marking marking) {
    os << (char)marking;
    return os;
}

/** Überladener <<-Operator für Player */
std::ostream& operator<<(std::ostream& os, GameBoard::Player player) {
    os << (char)player;
    return os;
}
