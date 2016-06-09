#include <iostream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

class Board {
    private:
        int Xstate, Ostate;
        int openSpots;
        string winner;
        double outcomeWeight;
        double winsPossible;
        double movesPossible;
        vector<Board> possible;
        int movePos;
        int turnCount;

    public:

        // construct board
        Board(int Xstate=0, int Ostate=0, int turnCount=0) {
            this->Xstate = Xstate;
            this->Ostate = Ostate;
            winner = "";
            this->turnCount = turnCount;
        }

        // print out the board state numbers
        void print() {
            cout << "X: " << Xstate << endl;
            cout << "O: " << Ostate << endl;
        }

        // prompt user to make next move
        void getMove() {
            int input;

            cout << "Please enter a move for X: ";
            cin >> input;

            Xmove(input);
        }

        // take the current open spots and add x or o corresponding to depth
        void buildMoveTree() {
            int i;
            int possibleMove;
            Board *tempBoard;

            // build all possible boards and evaluate them
            openSpots = 511 - (Xstate | Ostate);

            //cout << "open spots: " << openSpots << endl;

            for (i = 0; i < 9; i++) {
                possibleMove = (int)(0.5 + pow(2, 8 - i));
                //cout << "i: " << i << "Possible move: " << possibleMove;
                if ((possibleMove & openSpots) != 0) {
                    //cout << " - possible move" << endl;
                    tempBoard = new Board(Xstate, Ostate, turnCount);
                    if ((turnCount % 2) == 0) {
                        tempBoard->Xmove(i);
                    }
                    else {
                        tempBoard->Omove(i);
                    }
                    possible.push_back(*tempBoard);
                }
                //cout << endl;
            }
        }

        void evaluateBoard() {

        	// NEED TO FIND A WAY TO KEEP DEEP GAMES
        	// FROM INFLUENCING THE DECISION AS MUCH AS IMMEDIATE GAMES
        	// LIKE IF OPPONENT CAN WIN NEXT MOVE THAT HAS TO BE PRIORITY

            vector<Board>::iterator iter;

            // leaf games that must be evaluated for a winner
            if (possible.begin() == possible.end()) {
                status();
                if (winner.compare("O") == 0) {
                    outcomeWeight = 1;
                }
                else if (winner.compare("X") == 0) {
                	outcomeWeight = -1;
                }
                else {
                	outcomeWeight = 0;
                }
            }

            // the children must be evaluated and the weights averaged
            else {
            	double outcomeTotal = 0;
            	int outcomes = 0;
                for (iter = possible.begin(); iter != possible.end(); iter++) {
                    outcomeTotal += iter->outcomeWeight;
                    outcomes += 1;
                }
                outcomeWeight = outcomeTotal / outcomes;
            }
        }

        // takes a number 0-8 on board
        // top left, across and then down for numbering
        // put the new move in for O
        void Omove(int input) {
            int newMove;
            newMove = (int)(0.5 + pow(2, 8 - input));
            Ostate = Ostate | newMove;
            movePos = input;
            turnCount++;
        }
        // put the new move in for X
        void Xmove(int input) {
            int newMove;
            newMove = (int)(0.5 + pow(2, 8 - input));
            Xstate = Xstate | newMove;
            movePos = input;
            turnCount++;
        }

        void determineMove() {
            vector<Board>::iterator iter;

            if (!status()) {

                buildMoveTree();

                for (iter = possible.begin(); iter != possible.end(); iter++) {
                    //cout << "building move tree" << endl;
                    iter->determineMove();
                    //cout << "evaluating board" << endl;
                    iter->evaluateBoard();
                }
            }
        }

        // perform a monte carlo tree search
        // to determine next computer move
        void computerMove() {
            vector<Board>::iterator iter;

            //cout << "computing move choices" << endl;

            determineMove();

            //cout << "computing best move" << endl;

            // determine best chance of winning
            iter = possible.begin();
            int bestMove = iter->movePos;
            double bestProb = iter->outcomeWeight;
            for (iter = possible.begin(); iter != possible.end(); iter++) {
                if (bestProb <= (iter->outcomeWeight)) {
                    bestMove = iter->movePos;
                    bestProb = iter->outcomeWeight;
                }
            }
            cout << "win probability: " << bestProb << endl;

            Omove(bestMove);

            // for number of winning possibilities
        }

        // return true if game over
        // check if one set won
        // or if one side board full
        bool status() {
            if (((Xstate & 448) == 448) || ((Xstate & 56) == 56) ||
                ((Xstate & 7) == 7) || ((Xstate & 292) == 292) ||
                ((Xstate & 146) == 146) || ((Xstate & 73) == 73) ||
                ((Xstate & 273) == 273) || ((Xstate & 84) == 84)) {
                    winner = "X";
                }
            else if (((Ostate & 448) == 448) || ((Ostate & 56) == 56) ||
                ((Ostate & 7) == 7) || ((Ostate & 292) == 292) ||
                ((Ostate & 146) == 146) || ((Ostate & 73) == 73) ||
                ((Ostate & 273) == 273) || ((Ostate & 84) == 84)) {
                    winner = "O";
                }
            else if ((Xstate | Ostate) == 511) {
                winner = "Tie";
            }
            if (winner.compare("") != 0) {
                return true;
            }
            else return false;
        }

        string getWinner() {
            return winner;
        }

        int getTurnCount() {
        	return turnCount;
        }
};

int main()
{
   cout << "constructing board" << endl;
   Board *MyBoard = new Board(0, 0, 0);

   cout << "starting game" << endl;

   while ((MyBoard->getTurnCount() < 9) && (!(MyBoard->status()))) {

       cout << "next turn: " << (MyBoard->getTurnCount() % 2) << endl;
       MyBoard->print();

       if (((MyBoard->getTurnCount()) % 2) == 0) {
           // user moves
           MyBoard->getMove();
       }
       else {
           // computer moves
           MyBoard->computerMove();
       }
   }

   cout << "ending game" << endl;

   MyBoard->print();

   cout << "winner: " << MyBoard->getWinner() << endl;

   return 0;
}
