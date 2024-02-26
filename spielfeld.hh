#ifndef SPIELFELD_HH
#define SPIELFELD_HH

#include <iostream> /* für std::ostream */

class GameBoard {
public:
    enum Marking {
        Leer = '.',
        Marking_X = 'X',
        Marking_O = 'O'
    };

    enum Player {
        Player_X = Marking_X,
        Player_O = Marking_O
    };

    enum Scoring {
        Open,
        Draw,
        WinX,
        WinO
    };

    /** Konstruktor: Initialisiere das GameBoard */
    GameBoard();

    /** Methode: Gebe das GameBoard auf den Ausgabe-Stream os aus. */
    void printBoard(std::ostream& os);

    /** Prüfe, ob das angegebene Feld leer ist */
    bool fieldIsEmpty(int rows, int columns) { return position[rows][columns] == Leer; }

    /** Setze ein Feld */
    void setField(int rows, int columns, Player player);
    /** Nullung des gesetzten Feldes fuer miniMax()*/
    void resetField(int rows, int columns);

    /** Prüfe, ob es einen Gewinner gibt */
    Scoring gameScore();
private:
    /** Belegung des Spielfelds
     * position[rows][columns]: Zeile rows, Spalte columns
     */
    Marking position[3][3]{};

    /** Prüfe, ob der angegebene Player gewonnen hat */
    bool hasWon(Player player);

    /** Prüfe, ob alle Felder belegt sind */
    bool allFieldsTaken();

    /* Prüfe, ob es noch gewinnmoeglichkeiten gibt */
    bool winPossible();
};

/** Überladener <<-Operator für GameBoard-Markierungen */
std::ostream& operator<<(std::ostream& os, GameBoard::Marking marking);

/** Überladener <<-Operator für Player */
std::ostream& operator<<(std::ostream& os, GameBoard::Player player);

#endif /* SPIELFELD_HH */
