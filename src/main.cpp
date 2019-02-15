/*
    Simple Chess Engine

    This secretely represents my understanding on C++ Programming Language, about AI and Software Engineering

        Chess Engine At Its Core

    Ideals:
        - Simplicity
        - Flexibility
        - Maintenability
        - Modularity
        - Extensibility
        - Elegance

    Features:
        - Complete Chess Game
        - ASCII Interface
        - Evaluation Function
        - Artificial Intelligence
            - Minimax Algorithm
            - Alpha-beta Pruning

    Architecture:
        Board Representation
        Move Generation
        Move Search
        Move Evaluation

    Components:
        - Board Representation
            Board class
            Piece class
            Square class
            Move class
            Move List class

        - Move Generation Function
            Board class routine
                GenerateMoves(MoveList &moves)

        - Move Search Function
            AI class routine: Alpha-beta Pruning
                FindBestMove();
                Search(int alpha, int beta, int level)

        - Move Evaluation Function
            Materiality:
                Pawn    1
                Knight  3
                Bishop  3
                Rook    5
                Queen   9
                King    999

            Material Balance = Number of Pieces * Piece Value
            Mobility = Sum of Legal Moves
    
            Evaluation Function
                Utility = (Own Mobility - Opponent Mobility) + Material Balance * 100

    TODO:
        - En-Passant: bitwise
        - Castling Rights: White and Black: Long Castling, Short Castling; Rook, King, Check, use move history? use new data structure?
        - Resignation
        - Three-fold Repetition
        - Fifty Move Rule
        - Stalemate

    Future Upgrade:
        - Add GUI
            Minimal 2D Graphics

        - AI
            Learning Approach
                Artificial Neural Networks

            Heuristics Approach
                Transposition Table
                End-game Table
                Opening Book
                Negamax search
                Null-move heuristics

        - Data Structure
            0x88 Board
            Hashtable
        
        - Algorithm
            Legal-Move Generator

    Author: femto
    Created: 05/02/2019
    Last Updated: 12/02/2019
*/

#include <iostream>
#include "../inc/board.h"
#include "../inc/ai.h"

int main()
{
    std::cout << "Simple Chess Program Ver. Beta" << std::endl;
    
    // Load and Initialize Resources
    Board board;
    AI ai(board);

    board.Init();
    board.Display();

    // Game Loop
    while (true)
    {
        // Input
        board.Input();

        // AI
        ai.Update();

        // Game Logic
        board.Update();

        // Display
        board.Display();
    }

    return 0;
}