#include <iostream>
#include <string>

#include "spielfeld.hh"

#define HUMAN Spielfeld::Spieler_O
#define AI Spielfeld::Spieler_X

using namespace std;

struct SpielerStatistik{
    float gewinneSpielerX;
    float gewinneSpielerO;
};

bool zerlegeFeld(string eingabe, int& y, int& x) {
    //check if inputlength is valid
    if((eingabe.length() == 3 && eingabe[1] == ' ') || eingabe.length() == 2){
        if(eingabe.length() == 3){
            eingabe[1] = eingabe[2];
            eingabe[2] = '\0';//if space is in between, we remove it
        }
        for (int i = 0; i < eingabe.length(); i++) {
            switch (eingabe[i]) {
                case 'A':
                case 'B':
                case 'C':
                    x = eingabe[i] - 'A';
                    //cout << "x: " << x << endl;
                    break;
                case 'a':
                case 'b':
                case 'c':
                    x = eingabe[i] - 'a';
                    //cout << "x: " << x << endl;
                    break;
                case '1':
                case '2':
                case '3':
                    y = eingabe[i] - '1';
                    //cout << "y: " << y << endl;
                    break;
            }
        }return true;
    }return false;
}

/* Anderen Spieler bestimmen für Spielerwechsel */
Spielfeld::Spieler andererSpieler(Spielfeld::Spieler spieler) {
    switch (spieler) {
        case Spielfeld::Spieler_X: return Spielfeld::Spieler_O;
        case Spielfeld::Spieler_O: return Spielfeld::Spieler_X;
    }
}

void ausgabeSpielStatistik(Spielfeld& spielfeld, SpielerStatistik& spielerStatistik){
    cout << "--------------------------" << endl;
    cout << "Spielstatistik:" << endl;
    cout << "X: " << spielerStatistik.gewinneSpielerX << "x" << endl;
    cout << "O: " << spielerStatistik.gewinneSpielerO << "x" << endl;
    cout << "Spieler X hat " << spielerStatistik.gewinneSpielerX / (spielerStatistik.gewinneSpielerX + spielerStatistik.gewinneSpielerO) * 100 << "% der Spiele gewonnen." << endl;
    cout << "Spieler O hat " << spielerStatistik.gewinneSpielerO / (spielerStatistik.gewinneSpielerX + spielerStatistik.gewinneSpielerO) * 100 << "% der Spiele gewonnen." << endl;
}

int miniMax(Spielfeld spielfeld, int depth, bool isMaximizing){
    //check if game is over
    switch(spielfeld.spielstand()){
        case Spielfeld::Gewinn_O:
            return -10;
        case Spielfeld::Gewinn_X:
            return 10;
        case Spielfeld::Unentschieden:
            return 0;
    }

    if(isMaximizing){
        // AI is maximizing
        int bestScore = INT_MIN;
        for(int y = 0; y < 3; y++){
            for(int x = 0; x < 3; x++){
                if(spielfeld.feldIstLeer(y, x)){
                    spielfeld.setze(y, x, AI);
                    int score = miniMax(spielfeld, depth + 1, false);
                    spielfeld.setzeReset(y,x);
                    bestScore = max(score, bestScore);
                }
            }
        }
        return bestScore;
    }else{
        // HUMAN is minimizing
        int bestScore = INT_MAX;
        for(int y = 0; y < 3; y++){
            // iterate through the whole field
            for(int x = 0; x < 3; x++){
                if(spielfeld.feldIstLeer(y, x)){
                    spielfeld.setze(y, x, HUMAN);
                    int score = miniMax(spielfeld, depth + 1, true);
                    spielfeld.setzeReset(y,x);
                    bestScore = min(score, bestScore);
                }
            }
        }
        return bestScore;
    }
}

void bestMove(Spielfeld& spielfeld, Spielfeld::Spieler& aktuellerSpieler){
    int score = 0;
    int bestScore = INT_MIN;
    //struct for best move
    struct Move{
        int y;
        int x;
    };
    Move move{};
    //iterate through the whole field
    for(int y = 0; y < 3; y++){
        for(int x = 0; x < 3; x++){
            //spot avaiable
            if(spielfeld.feldIstLeer(y, x)){
                spielfeld.setze(y, x, aktuellerSpieler);
                score = miniMax(spielfeld, 0, false);
                spielfeld.setzeReset(y, x);
                if(score > bestScore){
                    bestScore = score;
                    move = {y, x};
                }
            }
        }
    }
    spielfeld.setze(move.y, move.x, aktuellerSpieler);
}


void gameLoop(Spielfeld& spielfeld, Spielfeld::Spieler& aktuellerSpieler, Spielfeld::Spielstand& spielstand, SpielerStatistik& spielerStatistik){
    /* Führe so lange fort, wie der Spielstand offen ist */
    while ((spielstand = spielfeld.spielstand()) == Spielfeld::Offen) {
        /* Zeige Spielfeld */
        spielfeld.zeige(cout);
        /* Zeige Spieler */
        cout << aktuellerSpieler << " ist am Zug!" << endl;
        /* Frage Feld ab */

        if(aktuellerSpieler == HUMAN){
            cout << "Waehle ein Feld! (z.B. A3)" << endl;
            string eingabe;
            getline(cin, eingabe);
            /* Prüfe Feld */
            int x, y;
            if (zerlegeFeld(eingabe, y, x)) {
                /* Zulässiges Feld => Prüfe, ob es leer ist*/
                if (spielfeld.feldIstLeer(y, x)) {
                    /* Feld ist leer => Setze es und wechsle Spieler */
                    spielfeld.setze(y, x, aktuellerSpieler);
                    aktuellerSpieler = andererSpieler(aktuellerSpieler);
                } else {
                    cout << "Dieses Feld ist schon belegt!" << endl;
                    cout << "Versuche es noch einmal!" << endl;
                }
            } else {
                cout << "Ein Feld besteht aus einem Buchstaben (A-C) gefolgt von einer Ziffer (1-3)!" << endl;
                cout << "Versuche es noch einmal!" << endl;
            }
        } else{ // AI
            bestMove(spielfeld, aktuellerSpieler);
            aktuellerSpieler = andererSpieler(aktuellerSpieler);
        }
    }
    switch (spielstand) {
        case Spielfeld::Gewinn_O:
            cout << "Spieler O hat gewonnen!" << endl;
            spielerStatistik.gewinneSpielerO++;
            aktuellerSpieler = HUMAN;
            ausgabeSpielStatistik(spielfeld, spielerStatistik);
            break;
        case Spielfeld::Gewinn_X:
            cout << "Spieler X hat gewonnen!" << endl;
            spielerStatistik.gewinneSpielerX++;
            aktuellerSpieler = AI;
            ausgabeSpielStatistik(spielfeld, spielerStatistik);
            break;
        case Spielfeld::Unentschieden:
            cout << "Unentschieden!" << endl;
            spielerStatistik.gewinneSpielerO += 0.5;
            spielerStatistik.gewinneSpielerX += 0.5;
            ausgabeSpielStatistik(spielfeld, spielerStatistik);
            break;
        default:
            break;
    }
}



int main() {
    Spielfeld spielfeld;
    Spielfeld::Spieler aktuellerSpieler = AI;
    Spielfeld::Spielstand spielstand;
    SpielerStatistik spielerStatistik= {.gewinneSpielerX =  0, .gewinneSpielerO =  0};

    bool weiterSpielen;
    do {
        gameLoop(spielfeld, aktuellerSpieler, spielstand, spielerStatistik);
        cout << "Nochmal spielen? (y/n)" << endl;
        string eingabe;
        getline(cin, eingabe);
        weiterSpielen = eingabe == "y";
        spielfeld = Spielfeld();
    } while (weiterSpielen);

    return 0;
}