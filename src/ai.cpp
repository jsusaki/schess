/* 
    Try out learning systems approach for newer versions
    Neural Networks
        Deep Learning
        Reinforcement Learning
*/

#include <stdlib.h> // exit()

#include "../inc/ai.h"
#include "../inc/movelist.h"


void AI::Update()
{    
    if (m_board.m_input.compare(0, 2, "go") == 0)
    {
        Move bestMove = FindBestMove();
        std::cout << "Best Move: " << bestMove << std::endl;
        m_board.MakeMove(bestMove);
        m_board.m_movehist.push_back(bestMove);
        m_board.m_ply++;
    }
}

/*
    Main AI
    
    Minimax Algorithm

    FindBestMove()
    It returns what it considers to be the best legal move in the current position.
*/

Move AI::FindBestMove()
{
    // Make a list of all legal moves
    MoveList moves;
    m_board.GenerateMoves(moves);
    
    // Breadth of search
    std::cout << "Info: " << moves.size() << " legal moves." << std::endl;

    // Make a list of the one or more best moves so far
    MoveList bestmoves; 
    
    // Assume the worst case
    int bestutil = -999;

    // Search through all legal moves
    for (unsigned int i = 0; i < moves.size(); i++)
    {
        Move move = moves[i];

        // Get value of current move
        m_board.MakeMove(move);
        int util = -Search(-999, 999, 4);    // Depth of search = 4
        m_board.UndoMove(move);

        std::cout << "Info: " << move << " : " << util << std::endl;

        // If new utility of move is greater than current best utility, new utility = best utility then put into best moves
        if (util > bestutil)
        {
            bestutil = util;
            bestmoves.clear();
            bestmoves.push_back(move);
        }

        // If new utility of move is equal to current best utility, put into best moves
        else if (util == bestutil)
            bestmoves.push_back(move);
    }

    // Debug
    if (bestmoves.size() == 0)
    {
        std::cout << "BUG: No legal moves!" << std::endl;
        exit(1);
    }

    return bestmoves[rand() % bestmoves.size()];
}

/*
    Alpha-Beta Pruning

    Search()

    This is a very basic (Fail-Soft Negamax) alpha-beta search.
    
    For details:
        http://chessprogramming.wikispaces.com/Alpha-Beta
        http://en.wikipedia.org/wiki/Alpha-beta_pruning

    Parameter values:
        alpha
        beta 
        level : The number of ply (half-moves) to search.

    The value returned by this function is: 
        util > 0 : positive, if the side to move has an advantage (is winning). 
        util < 0 : negative, if the side to move is losing.
        0        : zero, if the position is equal.
        +900     : if the opponent is check-mated.
        -999     : if the side to move is check-mated.
*/

int AI::Search(int alpha, int beta, int level)
{
     // We are at leaf node, just return the static evaluation
    if (level == 0)
        return m_board.EvaluatePosition();

    // Create a list of legal moves
    MoveList moves;
    m_board.GenerateMoves(moves);

    // Assume the worst
    int bestutil = -999;

    // Search through all legal moves
    for (unsigned int i = 0; i < moves.size(); ++i)
    {
        // This is the alpha-beta pruning.
        // Stop searching: if we already have found a "killer" move.
        if (bestutil >= beta)
            break;

        // This is part of the alpha-beta pruning too.
        // Tighten the search window.
        if (bestutil > alpha) 
            alpha = bestutil;
        
        const Move &move = moves[i];

        // Opponent's king can be captured. That means he is check-mated.
        if (move.IsKingCaptured())
            return 900 + level; 

        // Do a recursive search
        m_board.MakeMove(move);
        int util = -Search(-beta, -alpha, level-1);
        m_board.UndoMove(move);

        // Store the best value so far.
        if (util > bestutil)
            bestutil = util;
    }

    return bestutil;
}