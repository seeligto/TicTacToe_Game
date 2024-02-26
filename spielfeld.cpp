#include "spielfeld.hh"
#include <iomanip> /* für setw() */

/* Macht das Tippen leichter */
using namespace std;

Spielfeld::Spielfeld() {
    /* Initialisiere Spielfeld */
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            belegung[y][x] = Leer;
        }
    }
}

void Spielfeld::zeige(ostream& os) {
    /* Spaltenkopf */
    os << "  ABC" << endl;
    for (int y = 0; y < 3; y++) {
        /* Zeilenkopf */
        os << setw(1) << (y+1) << ' ';
        /* Zeileninhalt */
        for (int x = 0; x < 3; x++) {
            os << belegung[y][x];
        }
        os << endl;
    }
}

void Spielfeld::setze(int y, int x, Spieler spieler) {
    if (0 <= y && y <= 2 && 0 <= x && x <= 2) {
        belegung[y][x] = (Markierung)spieler;
    }
}

void Spielfeld::setzeReset(int y, int x) {
    if (0 <= y && y <= 2 && 0 <= x && x <= 2) {
        belegung[y][x] = Leer;
    }
}

Spielfeld::Spielstand Spielfeld::spielstand() {
    if (hatGewonnen(Spieler_O)) {
        return Gewinn_O;
    } else if (hatGewonnen(Spieler_X)) {
        return Gewinn_X;
    } else if (alleFelderBelegt()) {
        return Unentschieden;
    } else {
        return Offen;
    }
}

bool Spielfeld::hatGewonnen(Spieler spieler) {
    // Zeile belegt?
    for (int y = 0; y < 3; y++) {
        if (belegung[y][0] == (Markierung)spieler &&
            belegung[y][1] == (Markierung)spieler &&
            belegung[y][2] == (Markierung)spieler) {
                return true;
        }
    }
    // Spalte belegt?
    for (int x = 0; x < 3; x++) {
        if (belegung[0][x] == (Markierung)spieler &&
            belegung[1][x] == (Markierung)spieler &&
            belegung[2][x] == (Markierung)spieler) {
                return true;
        }
    }
    // 1. Hauptdiagonale belegt?
    if (belegung[0][0] == (Markierung)spieler &&
        belegung[1][1] == (Markierung)spieler &&
        belegung[2][2] == (Markierung)spieler) {
            return true;
    }
    // 2. Hauptdiagonale belegt?
    if (belegung[0][2] == (Markierung)spieler &&
        belegung[1][1] == (Markierung)spieler &&
        belegung[2][0] == (Markierung)spieler) {
            return true;
    }
    return false;
}

bool Spielfeld::ganzeReihe(int y, Spieler spieler) {
    // TODO
}

bool Spielfeld::ganzeSpalte(int x, Spieler spieler) {
    // TODO
}

bool Spielfeld::alleFelderBelegt() {
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (belegung[y][x] == Leer) {
                return false;
            }
        }
    }
    return true;
}

/** Überladener <<-Operator für Spielfeld-Markierungen */
std::ostream& operator<<(std::ostream& os, Spielfeld::Markierung markierung) {
    os << (char)markierung;
    return os;
}

/** Überladener <<-Operator für Spieler */
std::ostream& operator<<(std::ostream& os, Spielfeld::Spieler spieler) {
    os << (char)spieler;
    return os;
}
