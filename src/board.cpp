#include <iostream>
#include "../inc/board.h"


void Board::Init()
{
    e_piece initial[120] = 
    {
        IV, IV, IV, IV, IV, IV, IV, IV, IV, IV,
        IV, IV, IV, IV, IV, IV, IV, IV, IV, IV,
        IV, WR, WN, WB, WQ, WK, WB, WN, WR, IV,
        IV, WP, WP, WP, WP, WP, WP, WP, WP, IV,
        IV, EM, EM, EM, EM, EM, EM, EM, EM, IV,
        IV, EM, EM, EM, EM, EM, EM, EM, EM, IV,
        IV, EM, EM, EM, EM, EM, EM, EM, EM, IV,
        IV, EM, EM, EM, EM, EM, EM, EM, EM, IV,
        IV, BP, BP, BP, BP, BP, BP, BP, BP, IV,
        IV, BR, BN, BB, BQ, BK, BB, BN, BR, IV,
        IV, IV, IV, IV, IV, IV, IV, IV, IV, IV,
        IV, IV, IV, IV, IV, IV, IV, IV, IV, IV
    };

    m_board.reserve(120);

    for (int i = 0; i < 120; i++)
        m_board[i] = initial[i];

    m_side = 1;
    m_material = 0;
    m_ply = 0;
    m_move = 0;
    m_enPassantSquare = 0;
    m_castleRights = CASTLE_WHITE_SHORT | 
                     CASTLE_WHITE_LONG  | 
                     CASTLE_BLACK_SHORT | 
                     CASTLE_BLACK_LONG;
    m_state.clear();
}

void Board::Input()
{
    std::string input;
    std::cout << "Input Command: ";
    m_input = input;
    std::getline(std::cin, m_input);

/*
    Read and process input design?
    
    // Parse Command
    if (m_input.compare(0, 5, "move ") == 0)
    {
        // Do Move
    }

    // How to integrate AI in current design?
    else if (m_input.compare(0, 2, "go") == 0)
    {
        // AI
    }

    else if (m_input.compare(0, 4, "show") == 0)
    {
        // Show Legal Moves
    }

    else
        std::cout << "Invalid Command. Valid Commands are: move, go, show, " << std::endl;
*/
}
    
// Game Logic
void Board::Update()
{
    // Parse Command
    if (m_input.compare(0, 5, "move ") == 0)
    {
        Move move;
        
        // Parse Input
        // Move
        if (move.InputFromString(m_input.c_str()+5) == NULL)
            std::cout << "Invalid Input. Try algebraic notation e.g. e2e4" << std::endl;
        
        if (IsMoveValid(move))
        {
            MakeMove(move);
            bool check = IsOtherKingInCheck();
            UndoMove(move);
            
            if (check)
            {
                std::cout << "Your King is in CHECK. Play another move." << std::endl;
                return;
            }

            std::cout << "Your move: " << move << std::endl;
            MakeMove(move);
            m_movehist.push_back(move);
            m_ply++;
        }
        else 
            std::cout << "Move " << move << " is not legal." << std::endl;
    }

    // Show Legal Moves
    else if (m_input.compare(0, 4, "show") == 0)
    {
        MoveList moves;
        GenerateMoves(moves);
        std::cout << moves << std::endl;
    }

    // Show Move History
    else if (m_input.compare(0, 4, "hist") == 0)
    {
        for (int i = 0; i < m_movehist.size(); i++)
            std::cout << i + 1 << ". " << m_movehist[i].ToLongString() << std::endl;
    }
    
    // To not make annoying invalid input when we use go with AI
    else if (m_input.compare(0, 2, "go") == 0)
    {
    }
    
    // Quit
    else if (std::cin.eof() || m_input.compare(0, 4, "quit") == 0)
        exit(1);

    // Error
    else
        std::cout << "Unknown Command. Valid Commands are: move, go, show, hist, quit" << std::endl;
}

void Board::Display()
{
    // ASCII Graphics: it is called FEN notation; short for: Forsyth-Edward Notation
    // WHITE pieces are represented by upper case, and black pieces are represented by lower case
    static const char pieces[] = "kqrbnp.PNBRQK";
    
    // Board
    std::cout << std::endl;
    for (int file = 8; file >= 1; file--)
    {
        std::cout << file << " ";
        for (int rank = 1; rank <= 8; rank++)
        {
            int i = (file + 1) * 10 + rank;
            int piece = m_board[i];
            if (piece != IV) 
                std::cout << pieces[piece+6] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl;
    std::cout << std::endl;

    // Player Info
    switch (m_side)
    {
        case  1 : std::cout << "White to move" << std::endl; break;
        case -1 : std::cout << "Black to move" << std::endl; break;
    }
    
    // Castling Rights Info
    if (m_castleRights & CASTLE_WHITE_SHORT)
        std::cout << "K";
    if (m_castleRights & CASTLE_WHITE_LONG)
        std::cout << "Q";
    if (m_castleRights & CASTLE_BLACK_SHORT)
        std::cout << "k";
    if (m_castleRights & CASTLE_BLACK_LONG)
        std::cout << "q";
    if (m_castleRights)
        std::cout << "-" << " ";

    // En-Passant Square Info
    if (m_enPassantSquare)
        std::cout << Square(m_enPassantSquare);
        std::cout << std::endl;

    // Material Balance and Ply Info
    std::cout << "Material Balance: " << m_material << std::endl;
    std::cout << "Ply: " << m_ply << std::endl;

    // Check Info
    if (IsKingInCheck())
        std::cout << "Your King is in CHECK!" << std::endl;
}

// Check if current square is attacked
bool Board::IsSquareThreatened(const Square &sq) const
{
    if (m_side > 0) // Check for WHITE
    {
        // Check for pawns
        if (m_board[sq + NW] == BP || m_board[sq + NE] == BP)
            return true;

        // Check for knigts
        {
            int dirs[8] = { NNW, NNE, NWW, NEE, SSW, SSE, SWW, SEE };
            for (int k = 0; k < 8; k++)
            {
                int dir = dirs[k];
                if (m_board[sq + dir] == BN)
                    return true;
            }
        }

        // Check for diagonal (bishop and queen)
        {
            int dirs[4] = { NW, NE, SW, SE };
            for (int k = 0; k < 4; k++)
            {
                int dir = dirs[k];
                int sq2 = sq;
                while (1)
                {
                    sq2 += dir;
                    if (m_board[sq2] == BB || m_board[sq2] == BQ)
                        return true;
                    if (m_board[sq2] != EM)
                        break;
                }
            }
        }

        // Check for horizontal/vertical (rook and queen)
        {
            int dirs[4] = { N, S, W, E };
            for (int k = 0; k < 4; k++)
            {
                int dir = dirs[k];
                int sq2 = sq;
                while (1)
                {
                    sq2 += dir;
                    if (m_board[sq2] == BR || m_board[sq2] == BQ)
                        return true;
                    if (m_board[sq2] != EM)
                        break;
                }
            }
        }

        // Check for king
        {
            int dirs[8] = { NW, NE, SW, SE, N, S, W, E };
            for (int k = 0; k < 8; k++)
            {
                int dir = dirs[k];
                if (m_board[sq + dir] == BK) 
                    return true;
            }
        }
    }

    else // Check for black
    {
        // Check for pawns
        if (m_board[sq + SW] == WP || m_board[sq + SE] == WP)
            return true;

        // Check for knigts
        {
            int dirs[8] = { NNW, NNE, NWW, NEE, SSW, SSE, SWW, SEE };
            for (int k = 0; k < 8; k++)
            {
                int dir = dirs[k];
                if (m_board[sq + dir] == WN)
                    return true;
            }
        }

        // Check for diagonal (bishop and queen)
        {
            int dirs[4] = { NW, NE, SW, SE };
            for (int k = 0; k < 4; k++)
            {
                int dir = dirs[k];
                int sq2 = sq;
                while (1)
                {
                    sq2 += dir;
                    if (m_board[sq2] == WB || m_board[sq2] == WQ)
                        return true;
                    if (m_board[sq2] != EM)
                        break;
                }
            }
        }

        // Check for horizontal/vertical (rook and queen)
        {
            int dirs[4] = { N, S, W, E };
            for (int k = 0; k < 4; k++)
            {
                int dir = dirs[k];
                int sq2 = sq;
                while (1)
                {
                    sq2 += dir;
                    if (m_board[sq2] == WR || m_board[sq2] == WQ)
                        return true;
                    if (m_board[sq2] != EM)
                        break;
                }
            }
        }

        // Check for king
        {
            int dirs[8] = { NW, NE, SW, SE, N, S, W, E };
            for (int k = 0; k < 8; k++)
            {
                int dir = dirs[k];
                if (m_board[sq + dir] == WK)
                    return true;
            }
        }
    }
    return false;
}

void Board::GenerateMoves(MoveList &moves) const
{
    moves.clear();

    if (m_side > 0)
        for (int i = A1; i <= H8; i++)
        {
            int piece = m_board[i]; // source square; current position, where the piece lies
            int j;                  // target square; intended position

            switch (piece)
            {
                case WP : // White pawn
                    // One square forward
                    j = i + 10;
                    if (m_board[j] == EM)
                    {
                        // Check for promotion
                        if (i > 80)
                        {
                            {
                                Move move(piece, i, j, EM, WQ);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, EM, WR);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, EM, WB);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, EM, WN);
                                moves.push_back(move);
                            }
                        }

                        // Regular pawn move
                        else
                        {
                            Move move(piece, i, j, EM);
                            moves.push_back(move);
                        }
                        
                        // Two squares forward
                        j = i + 20;
                        if (m_board[j] == EM)
                        {
                            // Only from second rank
                            if (i < 40)
                            {
                                Move move(piece, i, j, EM);
                                moves.push_back(move);
                            }
                        }
                    }
                    
                    // Diagonal capture: NE
                    j = i + 9;
                    if ((m_board[j] != IV && m_board[j] < 0) || j == m_enPassantSquare)
                    {
                        // Check for promotion
                        if (i > 80)
                        {
                            {
                                Move move(piece, i, j, m_board[j], WQ);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, m_board[j], WR);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, m_board[j], WB);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, m_board[j], WN);
                                moves.push_back(move);
                            }
                        }

                        // Regular Pawn Move
                        else
                        {
                            Move move(piece, i, j, m_board[j]);
                            moves.push_back(move);
                        }
                    }

                    // Diagonal capture: NW
                    j = i + 11;
                    if ((m_board[j] != IV && m_board[j] < 0) || j == m_enPassantSquare)
                    {
                        // Check for promotion
                        if (i > 80)
                        {
                            {
                                Move move(piece, i, j, m_board[j], WQ);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, m_board[j], WR);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, m_board[j], WB);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, m_board[j], WN);
                                moves.push_back(move);
                            }
                        }

                        // Regular Pawn Move
                        else
                        {
                            Move move(piece, i, j, m_board[j]);
                            moves.push_back(move);
                        }
                    }
                    break;

                case WN : // White knight 
                    {
                        int dirs[8] = { NNW, NNE, NWW, NEE, SSW, SSE, SWW, SEE };

                        for (int k = 0; k < 8; k++)
                        {
                            int dir = dirs[k];

                            j = i + dir;
                            if (m_board[j] != IV) 
                                if (m_board[j] <= 0)
                                {
                                    Move move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                }
                        }
                    }
                    break;

                case WB : // White bishop
                    {
                        int dirs[4] = { NE, NW, SE, SW };

                        for (int k = 0; k < 4; k++)
                        {
                            int dir = dirs[k];
                            j = i;
                            while (1)
                            {
                                j += dir;
                                if (m_board[j] == IV) break;
                                if (m_board[j] <= 0)
                                {
                                    Move move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                    if (m_board[j] < 0)
                                        break;
                                }
                                else
                                    break;
                            }
                        }
                    }
                    break;

                case WR : // White rook
                    {
                        int dirs[4] = { N, E, S, W };

                        for (int k = 0; k < 4; k++)
                        {
                            int dir = dirs[k];
                            j = i;
                            while (1)
                            {
                                j += dir;
                                if (m_board[j] == IV) break;
                                if (m_board[j] <= 0)
                                {
                                    Move move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                    if (m_board[j] < 0)
                                        break;
                                }
                                else
                                    break;
                            }
                        }
                    }
                    break;

                case WQ : // White queen
                    {
                        int dirs[8] = { NE, NW, SE, SW, N, E, S, W };

                        for (int k = 0; k < 8; k++)
                        {
                            int dir = dirs[k];
                            j = i;
                            while (1)
                            {
                                j += dir;
                                if (m_board[j] == IV) break;
                                if (m_board[j] <= 0)
                                {
                                    Move move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                    if (m_board[j] < 0)
                                        break;
                                }
                                else
                                    break;
                            }
                        }
                    }
                    break;

                case WK : // White king
                    {
                        int dirs[8] = { NE, NW, SE, SW, N, E, S, W };

                        for (int k = 0; k < 8; k++)
                        {
                            int dir = dirs[k];

                            j = i + dir;
                            if (m_board[j] != IV) 
                                if (m_board[j] <= 0)
                                {
                                    Move move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                }
                        }

                        // Castling Short
                        if (m_castleRights & CASTLE_WHITE_SHORT)
                        {
                            // Then King and Rook must be in place
                            if (m_board[F1] == EM && m_board[G1] == EM)
                            {
                                if (!IsSquareThreatened(E1) && !IsSquareThreatened(F1))
                                {
                                    Move move(WK, E1, G1);
                                    moves.push_back(move);
                                }
                            }
                        }

                        // Castling Long
                        if (m_castleRights & CASTLE_WHITE_LONG)
                        {
                            // Then King and Rook must be in place
                            if (m_board[B1] == EM && m_board[C1] == EM && m_board[D1] == EM)
                            {
                                if (!IsSquareThreatened(E1) && !IsSquareThreatened(D1))
                                {
                                    Move move(WK, E1, C1);
                                    moves.push_back(move);
                                }
                            }
                        }
                    }
                    break;

                default : // Invalid, wrong color, or empty
                    continue;
            }
        }
    else
        for (int i = A1; i <= H8; i++)
        {
            int piece = m_board[i];
            int j;

            switch (piece)
            {
                case BP : // Black pawn
                    j = i - 10; // One squre forward
                    if (m_board[j] == EM)
                    {
                        if (i < 40) // Check for promotion
                        {
                            {
                                Move move(piece, i, j, EM, BQ);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, EM, BR);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, EM, BB);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, EM, BN);
                                moves.push_back(move);
                            }
                        }
                        else // Regular pawn move
                        {
                            Move move(piece, i, j, EM);
                            moves.push_back(move);
                        }

                        j = i - 20; // Two squares forward
                        if (m_board[j] == EM)
                        {
                            if (i > 80) // Only from seventh rank
                            {
                                Move move(piece, i, j, EM);
                                moves.push_back(move);
                            }
                        }
                    }

                    j = i - 9; // Diagonal capture
                    // defining color of pieces to not capture its own pieces. j > 0 means white piece
                    if ((m_board[j] != IV && m_board[j] > 0) || j == m_enPassantSquare)
                    {
                        if (i < 40) // Check for promotion
                        {
                            {
                                Move move(piece, i, j, m_board[j], BQ);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, m_board[j], BR);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, m_board[j], BB);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, m_board[j], BN);
                                moves.push_back(move);
                            }
                        }
                        else
                        {
                            Move move(piece, i, j, m_board[j]);
                            moves.push_back(move);
                        }
                    }

                    j = i - 11; // Diagonal capture
                    if ((m_board[j] != IV && m_board[j] > 0) || j == m_enPassantSquare)
                    {
                        if (i < 40) // Check for promotion
                        {
                            {
                                Move move(piece, i, j, m_board[j], BQ);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, m_board[j], BR);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, m_board[j], BB);
                                moves.push_back(move);
                            }
                            {
                                Move move(piece, i, j, m_board[j], BN);
                                moves.push_back(move);
                            }
                        }
                        else
                        {
                            Move move(piece, i, j, m_board[j]);
                            moves.push_back(move);
                        }
                    }
                    break;

                case BN : // Black knight
                    {
                        int dirs[8] = { NNW, NNE, NWW, NEE, SSW, SSE, SWW, SEE };

                        for (int k = 0; k < 8; k++)
                        {
                            int dir = dirs[k];

                            j = i + dir;
                            if (m_board[j] != IV) 
                                if (m_board[j] >= 0)
                                {
                                    Move move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                }
                        }
                    }
                    break;

                case BB : // Black bishop
                    {
                        int dirs[4] = { NE, NW, SE, SW };

                        for (int k = 0; k < 4; k++)
                        {
                            int dir = dirs[k];
                            j = i;
                            while (1)
                            {
                                j += dir;
                                if (m_board[j] == IV) break;
                                if (m_board[j] >= 0)
                                {
                                    Move move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                    if (m_board[j] > 0)
                                        break;
                                }
                                else
                                    break;
                            }
                        }
                    }
                    break;

                case BR : // Black rook
                    {
                        int dirs[4] = { N, E, S, W };

                        for (int k = 0; k < 4; k++)
                        {
                            int dir = dirs[k];
                            j = i;
                            while (1)
                            {
                                j += dir;
                                if (m_board[j] == IV) break;
                                if (m_board[j] >= 0)
                                {
                                    Move move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                    if (m_board[j] > 0)
                                        break;
                                }
                                else
                                    break;
                            }
                        }
                    }
                    break;

                case BQ : // Black queen
                    {
                        int dirs[8] = { NE, NW, SE, SW, N, E, S, W };

                        for (int k = 0; k < 8; k++)
                        {
                            int dir = dirs[k];
                            j = i;
                            while (1)
                            {
                                j += dir;
                                if (m_board[j] == IV) break;
                                if (m_board[j] >= 0)
                                {
                                    Move move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                    if (m_board[j] > 0)
                                        break;
                                }
                                else
                                    break;
                            }
                        }
                    }
                    break;

                case BK : // Black king
                    {
                        int dirs[8] = { NE, NW, SE, SW, N, E, S, W };

                        for (int k = 0; k < 8; k++)
                        {
                            int dir = dirs[k];
                            j = i + dir;

                            if (m_board[j] != IV) 
                                if (m_board[j] >= 0)
                                {
                                    Move move(piece, i, j, m_board[j]);
                                    moves.push_back(move);
                                }
                        }

                        // Castling Short
                        if (m_castleRights & CASTLE_BLACK_SHORT)
                        {
                            if (m_board[F8] == EM && m_board[G8] == EM)
                            {
                                if (!IsSquareThreatened(E8) && !IsSquareThreatened(F8))
                                {
                                    Move move(BK, E8, G8);
                                    moves.push_back(move);
                                }
                            }
                        }
                        // Castling Long
                        if (m_castleRights & CASTLE_BLACK_LONG)
                        {
                            if (m_board[B8] == EM && m_board[C8] == EM && m_board[D8] == EM)
                            {
                                if (!IsSquareThreatened(E8) && !IsSquareThreatened(F8))
                                {
                                    Move move(BK, E8, G8);
                                    moves.push_back(move);
                                }
                            }
                        }
                    }
                    break;

                default : // Invalid, wrong color, or empty
                    continue;
            }
        }
}

void Board::MakeMove(const Move &move)
{
    m_state.push_back((m_enPassantSquare << 8) | m_castleRights);
    m_enPassantSquare = 0;

    switch (move.GetCaptured())
    {
        case WP : case BP : m_material += 1; break;
        case WN : case BN : m_material += 3; break;
        case WB : case BB : m_material += 3; break;
        case WR : case BR : m_material += 5; break;
        case WQ : case BQ : m_material += 9; break;
        default : break;
    }

    switch (move.GetPromoted())
    {
        case WN : case BN : m_material += 3-1; break;
        case WB : case BB : m_material += 3-1; break;
        case WR : case BR : m_material += 5-1; break;
        case WQ : case BQ : m_material += 9-1; break;
        default : break;
    }

    // If rook is captured, then update castling rights
    switch (move.GetCaptured())
    {
        case WR :
            if (move.To() == H1)
                m_castleRights &= ~CASTLE_WHITE_SHORT;
            if (move.To() == A1)
                m_castleRights &= ~CASTLE_WHITE_LONG;
            break;
        case BR :
            if (move.To() == H8)
                m_castleRights &= ~CASTLE_BLACK_SHORT;
            if (move.To() == A8)
                m_castleRights &= ~CASTLE_BLACK_LONG;
            break;
        default : break;
    }

    // test with switch(move.From())
    switch (m_board[move.From()])
    {
        // Castling
        case WK :
            // White Castling Short
            if (move.From() == E1 && move.To() == G1)
            {
                m_board[H1] = EM;
                m_board[F1] = WR;
            }   
            // White Castling Long
            else if (move.From() == E1 && move.To() == C1)
            {
                m_board[A1] = EM;
                m_board[D1] = WR;
            }
            m_castleRights &= ~(CASTLE_WHITE_SHORT | CASTLE_WHITE_LONG);
            break;

        case BK:
            // Black Castling Short
            if (move.From() == E8 && move.To() == G8)
            {
                m_board[H8] = EM;
                m_board[F8] = BR;
            }
            // Black Castling Long
            else if (move.From() == E8 && move.To() == C8)
            {
                m_board[A8] = EM;
                m_board[D8] = BR;
            }
            m_castleRights &= ~(CASTLE_BLACK_SHORT | CASTLE_BLACK_LONG);
            break;

        case WR :
            if (move.From() == H1) // White moves king rook
                m_castleRights &= ~(CASTLE_WHITE_SHORT);
            else if (move.From() == A1) // White moves queen rook
                m_castleRights &= ~(CASTLE_WHITE_LONG);
            break;

        case BR :
            if (move.From() == H8) // White moves king rook
                m_castleRights &= ~(CASTLE_BLACK_SHORT);
            else if (move.From() == A8) // White moves queen rook
                m_castleRights &= ~(CASTLE_BLACK_LONG);
            break;

        // En-Passant
        case WP :
            if (move.To() - move.From() == 20) // White double pawn move
            {
                m_enPassantSquare = move.From() + 10;
            }
            else if (move.To() - move.From() != 10) // White pawn capture
            {
                if (m_board[move.To()] == EM)  // En-passant capture 
                {
                    m_board[move.To() - 10] = EM;
                }
            }
            break;

        case BP :
            if (move.From() - move.To() == 20) // Black double pawn move
            {
                m_enPassantSquare = move.To() + 10;
            }
            else if (move.From() - move.To() != 10) // Black pawn capture
            {
                if (m_board[move.To()] == EM)  // En-passant capture 
                {
                    m_board[move.To() + 10] = EM;
                }
            }
            break;
            
        default:
            break;
    }

    m_board[move.To()] = m_board[move.From()];
    if (move.GetPromoted() != EM)
        m_board[move.To()] = move.GetPromoted();
    m_board[move.From()] = EM;
    m_side = -m_side;
    m_material = -m_material;
}

void Board::UndoMove(const Move &move)
{
    m_material = -m_material;

    switch (move.GetCaptured())
    {
        case WP : case BP : m_material -= 1; break;
        case WN : case BN : m_material -= 3; break;
        case WB : case BB : m_material -= 3; break;
        case WR : case BR : m_material -= 5; break;
        case WQ : case BQ : m_material -= 9; break;
        default : break;
    }

    switch (move.GetPromoted())
    {
        case WN : case BN : m_material -= 3-1; break;
        case WB : case BB : m_material -= 3-1; break;
        case WR : case BR : m_material -= 5-1; break;
        case WQ : case BQ : m_material -= 9-1; break;
        default : break;
    }

    m_enPassantSquare = 0;
    switch (m_board[move.To()])
    {
        case WP :
            if ((move.To() - move.From()) % 10 != 0 && move.GetCaptured() == EM)
            {
                m_enPassantSquare = move.To();
                m_board[m_enPassantSquare - 10] = BP;
            }
            break;

        case BP :
            if ((move.From() - move.To()) % 10 != 0 && move.GetCaptured() == EM)
            {
                m_enPassantSquare = move.To();
                m_board[m_enPassantSquare + 10] = WP;
            }
            break;

        case WK :
            if (move.From() == E1 &&
                    move.To() == G1) // White castling short
            {
                m_board[F1] = EM;
                m_board[H1] = WR;
            }
            else if (move.From() == E1 &&
                    move.To() == C1) // White castling long
            {
                m_board[D1] = EM;
                m_board[A1] = WR;
            }
            break;

        case BK :
            if (move.From() == E8 &&
                    move.To() == G8) // Black castling short
            {
                m_board[F8] = EM;
                m_board[H8] = BR;
            }
            else if (move.From() == E8 &&
                    move.To() == C8) // Black castling long
            {
                m_board[D8] = EM;
                m_board[A8] = BR;
            }
            break;

        default :
            break;
    }

    m_board[move.From()] = move.GetPiece();
    m_board[move.To()] = move.GetCaptured();
    m_side = -m_side;
    
    if (!m_state.empty())
    {
        uint32_t state = m_state.back();
        m_enPassantSquare = state >> 8;
        m_castleRights = state & 0xFF;
        m_state.pop_back();
    }
    else
        m_enPassantSquare = 0;
}

bool Board::IsMoveValid(Move &move) const
{
    MoveList moves;
    GenerateMoves(moves);
    
    for (unsigned int i = 0; i < moves.size(); i++)
        if (moves[i] == move)
        {
            move.SetPiece(m_board[move.From()]);
            move.SetCaptured(m_board[move.To()]);
            return true;
        }

    return false;
}

// Simple Evaluation Function
int Board::EvaluatePosition()
{
    // Generate all possible moves for both white and black
    MoveList moves;

    GenerateMoves(moves);
    int my_moves = moves.size();
    SwapSide();

    GenerateMoves(moves);
    int opponent_moves = moves.size();
    SwapSide();
    
    // Calculate: Number of Moves + 100 * Material Balance 
    return (my_moves - opponent_moves) + 100 * m_material;
}

bool Board::IsKingInCheck() const
{
    e_piece king;
    Square kingSquare = 0;

    if (m_side > 0)
        king = WK;
    else
        king = BK;

    // Search for the king on the board
    for (int i = A1; i <= H8; i++)
        if (m_board[i] == king)
        {
            kingSquare = i;
            break;
        }

    return IsSquareThreatened(kingSquare);
}

bool Board::IsOtherKingInCheck() const
{
    ((Board *)this)->m_side = -m_side;
    bool retval = IsKingInCheck();
    ((Board *)this)->m_side = -m_side;
    return retval;
}