#pragma once

#include "piece.h"
#include "square.h"

class Move
{
public:
    friend class Board;

    // Constructors
    Move() : m_captured(IV), m_promoted(IV) {}
    Move(const char *str) : m_captured(IV) { InputFromString(str); }
    Move(const int piece, const Square &from, const Square &to, const int captured = EM, const int promoted = EM) :
        m_piece(piece), m_from(from), m_to(to), m_captured(captured), m_promoted(promoted) {}
    
    // Input
    const char *InputFromString(const char *s);

    // Output
    std::string ToShortString() const;
    std::string ToLongString() const;

    // Accessors
    Square From(void)               const { return m_from; }
    Square To(void)                 const { return m_to; }
    int GetPiece(void)              const { return m_piece; }
    int GetCaptured(void)           const { return m_captured; }
    int GetPromoted(void)           const { return m_promoted; }

    // Modifiers
    void SetPiece(int piece)        { m_piece = piece; }
    void SetCaptured(int captured)  { m_captured = captured; }
    void SetPromoted(int promoted)  { m_promoted = promoted; }

    // Utilities
    bool Valid(void)                const { return m_captured != IV; }
    bool IsKingCaptured(void)       const { return (m_captured == WK || m_captured == BK); }
    bool IsCaptured(void)           const { return m_captured != EM; }

    friend std::ostream &operator << (std::ostream &os, const Move &move);

    bool operator == (const Move &move) const
    {
        if (move.m_from != From()) return false;
        if (move.m_to != To()) return false;
        return true;
    }

private:
    Square m_from;
    Square m_to;
    int m_piece;
    int m_captured;
    int m_promoted;

};