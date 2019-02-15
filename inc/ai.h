#pragma once

#include "board.h"

class AI
{
public:
    AI(Board &board) : m_board(board) {}
    
    void Update();
    Move FindBestMove();

private:
    int Search(int alpha, int beta, int level);
    Board &m_board;

};