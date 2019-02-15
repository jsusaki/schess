/* 
    The board is represented as an array of 120 elements.
    arranged in 12 rows and 10 columns.
    The following diagram shows the numbering of the elements.
    Note: The square A1 is element number 21

    110 111 112 113 114 115 116 117 118 119
    100 101 102 103 104 105 106 107 108 109
     90  A8  B8  C8  D8  E8  F8  G8  H8  99
     80  A7  B7  C7  D7  E7  F7  G7  H7  89
     70  A6  B6  C6  D6  E6  F6  G6  H6  79
     60  A5  B5  C5  D5  E5  F5  G5  H5  69
     50  A4  B4  C4  D4  E4  F4  G4  H4  59
     40  A3  B3  C3  D3  E3  F3  G3  H3  49
     30  A2  B2  C2  D2  E2  F2  G2  H2  39
     20  A1  B1  C1  D1  E1  F1  G1  H1  29
     10  11  12  13  14  15  16  17  18  19
      0   1   2   3   4   5   6   7   8   9

    Each element contains one of the following values:
     0 :  Empty
     1 :  White Pawn
     2 :  White Knight
     3 :  White Bishop
     4 :  White Rook
     5 :  White Queen
     6 :  White King
    -1 :  Black Pawn
    -2 :  Black Knight
    -3 :  Black Bishop
    -4 :  Black Rook
    -5 :  Black Queen
    -6 :  Black King
    99 :  INVALID

    Each move performs index operations on its elements
    N   =  10, 
    S   = -10, 
    E   =  -1, 
    W   =   1,
    NW  =  11, 
    SW  =  -9, 
    NE  =   9, 
    SE  = -11,
    NNW =  21, 
    NNE =  19, 
    NWW =  12, 
    NEE =   8,
    SSW = -19, 
    SSE = -21, 
    SWW =  -8, 
    SEE = -12
*/

#pragma once

#include <vector>
#include <string>

#include "square.h"
#include "piece.h"
#include "move.h"
#include "movelist.h"


// Directions
enum
{
    N   =  10, 
    S   = -10, 
    E   =  -1, 
    W   =   1,
    NW  =  11, 
    SW  =  -9, 
    NE  =   9, 
    SE  = -11,
    NNW =  21, 
    NNE =  19, 
    NWW =  12, 
    NEE =   8,
    SSW = -19, 
    SSE = -21, 
    SWW =  -8, 
    SEE = -12
} e_directions;

class Board
{
public:
    // Constructor
    Board() {}
    Board(const Board &board);

    // General Method
    void Init();
    void Input();
    void Update();
    void Display();

    // Game Logic
    void MakeMove(const Move &move);
    void UndoMove(const Move &move);
    void GenerateMoves(MoveList &moves) const;
    bool IsMoveValid(Move &move) const;
    bool IsKingInCheck() const;
    bool IsOtherKingInCheck() const;

    // Evaluation Function
    int EvaluatePosition();

    friend std::ostream &operator << (std::ostream &os, const Board &board);

    std::string m_input;
    std::vector<Move> m_movehist;
    int m_ply;
private:
    std::vector<int> m_board;       // Board itself
    std::vector<uint32_t> m_state;  // Log for Castle Rights and En-Passant Square
    int m_material;
    int m_side; // 1 : WHITE -1 : BLACK

    bool IsSquareThreatened(const Square &sq) const;
    void SwapSide() { m_side = -m_side; }

    // Castling, En-Passant, Fifty-Move Rule, Threefold Repetition, Draw, Stalemate
    int m_castleRights;
#define CASTLE_WHITE_SHORT (1<<0)
#define CASTLE_WHITE_LONG  (1<<1)
#define CASTLE_BLACK_SHORT (1<<2)
#define CASTLE_BLACK_LONG  (1<<3)
    int m_enPassantSquare;
    int m_move;

};
