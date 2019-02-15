#pragma once

#include <iostream>

// Square Enumeration
enum
{
    A8 = 91, B8, C8, D8, E8, F8, G8, H8,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
} e_sq;

class Square
{
public:
    Square(int square = 0) : m_index(square) {}

    int File() const { return (m_index % 10); }
    int Rank() const { return (m_index / 10) - 1; }

    operator int() const { return m_index; }

    int InputFromString(const char *s)
    {
        m_file = s[0] - 'a';
        m_rank = s[1] - '1';
        m_index = m_rank * 10 + m_file + 21;
        return (m_file >= 8 || m_rank >= 8);
    }
    
    friend std::ostream& operator << (std::ostream &os, const Square &sq)
    {
        char file = (sq.m_index % 10) + 'a' - 1;
        char rank = (sq.m_index / 10) + '1' - 2;
        return os << file << rank;
    }

private:
    int m_index;
    int m_file;
    int m_rank;

};