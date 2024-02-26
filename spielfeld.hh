#ifndef SPIELFELD_HH
#define SPIELFELD_HH

#include <iostream> /* für std::ostream */

class Spielfeld {
public:
    enum Markierung {
        Leer = '.',
        Markierung_X = 'X',
        Markierung_O = 'O'
    };

    enum Spieler {
        Spieler_X = Markierung_X,
        Spieler_O = Markierung_O
    };

    enum Spielstand {
        Offen,
        Unentschieden,
        Gewinn_X,
        Gewinn_O
    };

    /** Konstruktor: Initialisiere das Spielfeld */
    Spielfeld();

    /** Methode: Gebe das Spielfeld auf den Ausgabe-Stream os aus. */
    void zeige(std::ostream& os);

    /** Prüfe, ob das angegebene Feld leer ist */
    bool feldIstLeer(int y, int x) { return belegung[y][x] == Leer; }

    /** Setze ein Feld */
    void setze(int y, int x, Spieler spieler);
    /** Nullung des gesetzten Feldes fuer miniMax()*/
    void setzeReset(int y, int x);

    /** Prüfe, ob es einen Gewinner gibt */
    Spielstand spielstand();
private:
    /** Belegung des Spielfelds
     * belegung[y][x]: Zeile y, Spalte x
     */
    Markierung belegung[3][3]{};

    /** Prüfe, ob der angegebene Spieler gewonnen hat */
    bool hatGewonnen(Spieler spieler);

    /** Prüfe, ob der gegebene Spieler die ganze Reihe belegt hat */
    bool ganzeReihe(int y, Spieler spieler);

    /** Prüfe, ob der gegebene Spieler die ganze Spalte belegt hat */
    bool ganzeSpalte(int x, Spieler spieler);

    /** Prüfe, ob alle Felder belegt sind */
    bool alleFelderBelegt();
};

/** Überladener <<-Operator für Spielfeld-Markierungen */
std::ostream& operator<<(std::ostream& os, Spielfeld::Markierung markierung);

/** Überladener <<-Operator für Spieler */
std::ostream& operator<<(std::ostream& os, Spielfeld::Spieler spieler);

#endif /* SPIELFELD_HH */
