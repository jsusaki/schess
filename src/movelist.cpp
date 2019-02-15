#include "../inc/movelist.h"


std::ostream& operator << (std::ostream &os, const MoveList &moves)
{
    int i = 1;
    for (std::vector<Move>::const_iterator it = moves.m_moveList.begin(); it != moves.m_moveList.end(); it++)
        os << i++ << ". " << (*it).ToLongString() << std::endl;
    return os;
}