# Connect 4 Game

This is a C++ console-based implementation of the classic game Connect 4. The game allows two players to take turns placing their tokens (X and O) in a grid with the goal of forming a line of four tokens in a row, column, or diagonal. The game also provides the option to save and load games and keeps track of top players' scores.

## Features

- Two-player mode: Play against a friend on the same computer.
- Single-player mode: Play against the computer (random move AI).
- Save and load games: Save your progress and continue playing later.
- Top players: Keep track of top player scores.

## How to Play

1. Run the program.
2. Choose one of the following options from the menu:
   - New two-player game: Start a new game for two players.
   - New one-player game: Start a new single-player game against the computer.
   - Load Game: Load a previously saved game.
   - Print Top Players: View the top players and their scores.
   - Exit: Quit the game.
3. During gameplay, players take turns selecting columns to drop their tokens into.
4. The game ends when a the grid completed and the winner who has larger four tokens in a row, column, or diagonal.
5. In single-player mode, you play against a random-move  .

## Saving Games

The program saves the game state using FEN (Forsyth-Edwards Notation), a standard format for representing chess positions. The saved games are stored in a file named "SavedGames.txt."

## Top Players

The program keeps track of the top players' scores and displays them in descending order. You can view the top players from the menu option "Print Top Players."

## Dependencies

- This program is written in C++ and uses standard C++ libraries.
- It is designed to run on a Windows system due to its use of Windows console functions for text colorization.

## Usage

1. Compile the program.
2. Run the compiled executable (`Connect4Game.exe` on Windows).
3. Follow the on-screen instructions to navigate the menu and play the game.

Enjoy playing Connect 4
