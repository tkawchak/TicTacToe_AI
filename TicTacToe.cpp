#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Board {
    private:
        int Xstate, Ostate;  // 9-bit mask for current positions of each player
        int openSpots;  // 9-bit mask for open spots on board
        string winner;
        int outcomeWeight;
        vector<Board> possible;
        int movePos;
        int turnCount;

    public:

        // construct board
        Board(int Xstate=0, int Ostate=0, int turnCount=0) {
            this->Xstate = Xstate;
            this->Ostate = Ostate;
            this->winner = "";
            this->turnCount = turnCount;
            this->outcomeWeight = 0;
        }

        // print out the board state numbers
        void print() {
        	string position[9];
        	ostringstream numString;
        	int possibleMove;
        	int i;

        	for (i = 0; i < 9; i++) {
        		numString.str("");
        		numString.clear();
				possibleMove = (int)(0.5 + pow(2, 8 - i));

				if (possibleMove & Xstate) {
					position[i] = "X";
				}
				else if (possibleMove & Ostate) {
					position[i] = "O";
				}
				else {
					numString << i;
					position[i] = numString.str();
				}

			}

        	cout << "   |   |   " << endl;
        	cout << " " << position[0] << " | " << position[1] << " | " << position[2] << " " << endl;
        	cout << "   |   |   " << endl;
        	cout << "---+---+---" << endl;
        	cout << "   |   |   " << endl;
        	cout << " " << position[3] << " | " << position[4] << " | " << position[5] << " " << endl;
        	cout << "   |   |   " << endl;
        	cout << "---+---+---" << endl;
        	cout << "   |   |   " << endl;
        	cout << " " << position[6] << " | " << position[7] << " | " << position[8] << " " << endl;
        	cout << "   |   |   " << endl;
        }

        // prompt user to make next move
        void getMove() {
            int input;
            cout << "Please enter a move for X: ";
            cin >> input;
            Xmove(input);
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

        // build next level of move tree
        void buildMoveTree() {
        	cout << "building move tree." << endl;
            int i;
            int possibleMove;
            Board *tempBoard;

            // build all possible boards and evaluate them
            openSpots = getOpenSpots();

            for (i = 0; i < 9; i++) {

                possibleMove = (int)(0.5 + pow(2, 8 - i));

                if ((possibleMove & openSpots) != 0) {
                    tempBoard = new Board(Xstate, Ostate, turnCount);
                    if ((turnCount % 2) == 0) {
                        tempBoard->Xmove(i);
                    }
                    else {
                        tempBoard->Omove(i);
                    }
                    possible.push_back(*tempBoard);
                }
            }
        }

        // ****************************************
        // algorithm to implement
        // build possible move tree for one row
        // evaluate to see if anything in that row results in a win
        // if so, then return that result as the choice
        // if not, then
        	// if we can build more trees then call function to build and evaluate possible row of subtree
        	// else evaluate the trees and return the subtree that maximizes winning
        // *******************************************************


        // evaluate the leaf node of a game
        // set the outcomeWeight to a valid weight
        void evaluateLeaf() {
        	cout << "evaluating leaf." << endl;
        	// leaf games that must be evaluated for a winner
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
			cout << "exiting evaluate leaf." << endl;
        }

        // evaluate next level of board and return the
        // index corresponding to next move choice
        // return 9 if other moves must be built
        // set the outcomeWeight of the current game
        int evaluateBoard() {
            vector<Board>::iterator iter;
            int bestMove = 9;

            iter = possible.begin();

            cout << "evaluating board." << endl;

            // evaluate leaf node for win value
            if (status() && (iter == possible.end())) {
            	evaluateLeaf();
            	bestMove = movePos;
            	cout << "returning from evaluateBoard leaf: " << bestMove << endl;
            	return bestMove;
            }

            // return 9 if the subtree has not been built yet
            else if ((iter == possible.end())) {
            	cout << "returning from evaluateBoard: " << bestMove << endl;
            	return bestMove;
            }

            // the children must be evaluated
            // if the opponent can win in the child then we must assume it will.
            else {

            	// initialize the weights to worst case scenario
            	if ((turnCount % 2) == 0) {
            		outcomeWeight = 1;
            	}
            	else if ((turnCount % 2) == 1) {
            		outcomeWeight = -1;
            	}

                for (iter = possible.begin(); iter != possible.end(); iter++) {

                	// MAYBE CHANGE THIS TO CONSIDER ALL POSSIBLE MOVES THAT WOULD RESULT
                	// IN A MAXIMIZED SCORE?? RIGHT NOW I JUST PICK THE LAST ONE

					// X turn
					if ((turnCount % 2) == 0) {
						if (iter->outcomeWeight <= outcomeWeight) {
							outcomeWeight = iter->outcomeWeight;
							bestMove = iter->movePos;
						}
					}
					// O turn
					else if ((turnCount % 2) == 1) {
						if (iter->outcomeWeight >= outcomeWeight) {
							outcomeWeight = iter->outcomeWeight;
							bestMove = iter->movePos;
						}
					}
                	cout << "weight: " << iter->outcomeWeight << endl;
                }
            	cout << "returning from evaluateBoard: " << bestMove << endl;
                return bestMove;
            }
        }


        // determine the next move for the program to make
        // evaluates the current game
        // builds more games if necessary
        // repeats this process on the children
        int determineMove() {
        	cout << "determine move." << endl;
            vector<Board>::iterator iter;
            int bestMove = 9;

            bestMove = evaluateBoard();
            if (bestMove == 9) {
            	buildMoveTree();
            }
            else {
            	return bestMove;
            }

            // recursively call determine move function to evaluate and build subtrees as necessary
			for (iter = possible.begin(); iter != possible.end(); iter++) {
				iter->determineMove();
			}

			bestMove = evaluateBoard();
			cout << "returning from determine move: " << bestMove << endl;
            return bestMove;
        }

        // perform a monte carlo tree search
        // to determine next computer move
        // INCLUDE a-B PRUNING TECHNIQUES HERE
        // TO IMPROVE THE ALGORITHM
        void computerMove() {
        	cout << "getting computer move." << endl;
            vector<Board>::iterator iter;
            int movePos;

            movePos = determineMove();

            if ((turnCount % 2) == 0) {
            	Xmove(movePos);
            }
            else {
            	Omove(movePos);
            }
        }

        // return true if game over, false if not over
        // determine winner by setting value of winner
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

        int getOpenSpots() {
        	return 511 - (Xstate | Ostate);
        }
};

int main()
{
   cout << "constructing board..." << endl;
   Board *MyBoard = new Board(282, 133, 7);
   cout << "starting game..." << endl;

   MyBoard->print();
   MyBoard->computerMove();

   while ((MyBoard->getTurnCount() < 9) && (!(MyBoard->status()))) {
	   cout << endl;
       MyBoard->print();
       cout << "next turn: " << (MyBoard->getTurnCount() % 2) << endl;
       if (((MyBoard->getTurnCount()) % 2) == 0) {
           // user moves
           MyBoard->getMove();
       }
       else {
           // computer moves
           MyBoard->computerMove();
       }
   }

   cout << "ending game..." << endl;
   MyBoard->print();
   MyBoard->status();
   cout << "winner: " << MyBoard->getWinner() << endl;
   return 0;
}
