#TicTacToe Program

This is a Tic Tac Toe program that will play against you with an AI.  This is written in C++ with no dependencies besides the standard libraries and uses decision trees to determine subsequent moves.

Currently the AI needs some work but you will be able to play a game.
To enter a number, select a number from 0-8 that corresponds to a position on the board.  Numbering starts at the top-left side and works across rows and then down.
The current board status will be returned as a number from 0-511, as a bitmask of current moves on the board.  LSB is the bottom right corner and MSB is top left corner.  Basically, the opposite numbering scheme of the move selection.

#Improvements Coming:
Improve AI
Improve Ease of Use
Refactor code

Note: I will not be adding a GUI.  This is just a sample project for me to learn about game AI.
