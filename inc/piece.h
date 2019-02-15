#pragma once

// Piece Representation
typedef enum
{
    EM =  0,  // Empty
    WP =  1,  // White Pawn
    WN =  2,  // White Knight
    WB =  3,  // White Bishop
    WR =  4,  // White Rook
    WQ =  5,  // White Queen
    WK =  6,  // White King
    BP = -1,  // Black Pawn
    BN = -2,  // Black Knight
    BB = -3,  // Black Bishop
    BR = -4,  // Black Rook
    BQ = -5,  // Black Queen
    BK = -6,  // Black King
    IV = 99   // INVALID
} e_piece;