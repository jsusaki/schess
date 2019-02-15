#pragma once

#include <iostream>
#include <vector>
#include "move.h"


class MoveList
{
public:
    friend std::ostream& operator << (std::ostream &os, const MoveList &moves);
    
    MoveList()
    {
        m_moveList.reserve(100);
    }

    void push_back(const Move& move)
    {
        m_moveList.push_back(move);
    }

    void clear()
    {
        m_moveList.clear();
    }

    unsigned int size() const
    {
        return m_moveList.size();
    }

    const Move & operator [] (unsigned int ix) const { return m_moveList[ix]; }

private:
    std::vector<Move> m_moveList;

};