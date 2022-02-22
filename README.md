# Sliding Puzzle

A [sliding puzzle](https://en.wikipedia.org/wiki/Sliding_puzzle) in this context defines a `n x m`-sized board filled with unordered numbers and one empty field. Only numbers adjacent to the empty spot can be moved to that place, effectively exchanging the position of the empty field and the moved number. The goal is to sort the numbers (reading from left to right and top to bottm) in the correct order in the least moves possible.

Instead of numbers, pictres or others can be used instead, but for our digital representation integers from 1 to *n*m-1* shall be used.

This program is meant to do several things. Using the command line, the user is meant to decide whether they want to play a sliding puzzle themselves or whether they want the computer to solve one. During any point of time of playing a sliding puzzle, the player should have the possibility of letting the computer finish the game for them.

As a last, potential feature, the player should input puzzles themselves, either by giving the computer a puzzle-size and a minimum of moves or by actually inputting the initial puzzle state.


# Board Representation:

Board size is stored as `m x n`, where m is the column size and n is the row size.

Example board for `4 x 3`, where each value x is equivalent to its index i (stored linearly in memory)

0   1   2   3

4   5   6   7

8   9   10  11


The position of any value x with the index i is the following:

- Column: `(int) i / m`
- Row: `i % m`

The only valid solution in the current implementation is one, where the empty field is in the last position of the board.