#include <sstream>
#include <ctype.h>

#include "../inc/move.h"


std::ostream &operator << (std::ostream &os, const Move &move)
{
    return os << move.ToShortString();
}

static char pieces[] = "kqrbnp.PNBRQK";

std::string Move::ToShortString() const
{
    std::stringstream ss;
    ss << m_from;
    ss << m_to;

    if (GetPromoted() != EM)
    {
        ss << (char) tolower(pieces[GetPromoted()+6]);
    }
    return ss.str();
}

std::string Move::ToLongString() const
{
    std::stringstream ss;
    ss << pieces[GetPiece()+6];
    ss << m_from;
    
    if (GetCaptured() != EM)
        ss << "*";
    else
        ss << "-";
    ss << m_to;

    if (GetPromoted() != EM)
    {
        ss << "=";
        ss << (char) tolower(pieces[GetPromoted()+6]);
    }
    return ss.str();
}

const char *Move::InputFromString(const char *s)
{
    // Check if input is valid
    if (m_from.InputFromString(s) || m_to.InputFromString(s+2))
        return NULL;
    
    s += 4;

    SetPiece(EM);
    SetCaptured(EM);
    SetPromoted(EM);

    // Check for Black Pawn Promotion
    if (m_to.Rank() == 1)
    {
        switch (tolower(s[0]))
        {
            case 'q' : SetPromoted(BQ); s++; break;
            case 'r' : SetPromoted(BR); s++; break;
            case 'b' : SetPromoted(BB); s++; break;
            case 'n' : SetPromoted(BN); s++; break;
            case ' ' : break;
            case '\0': break;
            default  : return NULL;
        }
    }
    
    // Check for White Pawn Promotion
    else if (m_to.Rank() == 8)
    {
        switch (tolower(s[0]))
        {
            case 'q' : SetPromoted(WQ); s++; break;
            case 'r' : SetPromoted(WR); s++; break;
            case 'b' : SetPromoted(WB); s++; break;
            case 'n' : SetPromoted(WN); s++; break;
            case ' ' : break;
            case '\0': break;
            default  : return NULL;
        }
    }

    while (s[0] == ' ')
        s++;
    return s;
}