#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>

const int FACTORIAL[] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320 };

const unsigned char SOLVED_CENTER_POS[] = { 0, 1, 2, 3, 4, 5 };
const unsigned char SOLVED_CORNER_POS[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
const unsigned char SOLVED_CORNER_ROT[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

class Skewb
{
public:
    // Corner names
    //
    // L\     /R
    // | \   / |
    // |  \U/  |
    // |   |   |
    //  \  |  /
    //   \ | /
    //    \D/

    // C is clockwise and CC is counterclockwise.
    // The bottom back corner is fixed.

    enum move_t
    {
        UP_C    =  (1 << 1),
        UP_CC,
        DOWN_C  =  (1 << 2),
        DOWN_CC,
        RIGHT_C =  (1 << 3),
        RIGHT_CC,
        LEFT_C  =  (1 << 4),
        LEFT_CC
    };

    Skewb() { reset(); }
    explicit Skewb(uint64_t stateNum) { setStateNum(stateNum); }
    explicit Skewb(const Skewb& other) { set(other.m_centerPos, other.m_cornerPos, other.m_cornerRot); }
    Skewb(const unsigned char* centerPos, const unsigned char* cornerPos, const unsigned char* cornerRot) { set(centerPos, cornerPos, cornerRot); }

    Skewb& operator=(const Skewb& other) { set(other.m_centerPos, other.m_cornerPos, other.m_cornerRot); return *this; }

    void set(const unsigned char* centerPos, const unsigned char* cornerPos, const unsigned char* cornerRot);
    void reset() { set(SOLVED_CENTER_POS, SOLVED_CORNER_POS,  SOLVED_CORNER_ROT); }

    void setStateNum(uint64_t stateNum);
    uint64_t getStateNum() const;

    void makeMove(move_t move);

    bool isSolved() const;

    std::string getStateStr() const;

private:
    unsigned char m_centerPos[6];
    unsigned char m_cornerPos[8];
    unsigned char m_cornerRot[8];
};

void Skewb::set(const unsigned char* centerPos, const unsigned char* cornerPos, const unsigned char* cornerRot)
{
    memcpy(m_centerPos, centerPos, 6);
    memcpy(m_cornerPos, cornerPos, 8);
    memcpy(m_cornerRot, cornerRot, 8);
}

void Skewb::setStateNum(uint64_t stateNum)
{
    for (int i = 0; i < 8; i++)
    {
        m_cornerRot[5 - i] = stateNum % 3;
        stateNum /= 3;
    }

    m_cornerPos[7] = 0;
    for (int i = 2; i <= 8; i++)
    {
        m_cornerPos[8 - i] = stateNum % i;
        stateNum /= i;
    }
    for (int i = 6; i >= 0; i--)
        for (int j = i + 1; j < 8; j++)
            if (m_cornerPos[j] >= m_cornerPos[i]) m_cornerPos[j]++;

    m_centerPos[5] = 0;
    for (int i = 2; i <= 6; i++)
    {
        m_centerPos[6 - i] = stateNum % i + 1;
        stateNum /= i;
    }
    for (int i = 4; i >= 0; i--)
        for (int j = i + i; j < 6; j++)
            if (m_centerPos[j] >= m_centerPos[i]) m_centerPos[j]++;
}

uint64_t Skewb::getStateNum() const
{
    uint64_t n = 0;

    unsigned char centerPos[5];
    memcpy(centerPos, m_centerPos, 5);
    for (int i = 0; i < 4; i++)
        for (int j = i + 1; j < 5; j++)
            if (centerPos[j] > centerPos[i]) centerPos[j]--;
    for (int i = 0; i < 5; i++)
        n = n * (6 - i) + centerPos[i];
/*
    unsigned char cornerPos[8];
    memcpy(cornerPos, m_cornerPos, 8);
    for (int i = 0; i < 7; i++)
        for (int j = i + 1; j < 8; j++)
            if (cornerPos[j] > cornerPos[i]) cornerPos[j]--;
    for (int i = 0; i < 8; i++)
        n = n * (8 - i) + cornerPos[i];

    for (int i = 0; i < 8; i++)
        n = n * 3 + m_cornerRot[i];
*/
    return n;
}

void Skewb::makeMove(move_t move)
{
}

bool Skewb::isSolved() const
{
    for (int i = 0; i < 6; i++) { if (m_centerPos[i] != i) return false; }
    for (int i = 0; i < 8; i++) { if (m_cornerPos[i] != i) return false; }
    for (int i = 0; i < 8; i++) { if (m_cornerRot[i] != 0) return false; }
    return true;
}

std::string Skewb::getStateStr() const
{
    std::stringstream ss;
    ss << "centerpos: { ";
    bool bAddComma = false;
    for (int i = 0; i < 6; i++)
    {
        if (bAddComma) ss << ", ";
        else bAddComma = true;
        ss << (int)m_centerPos[i];
    }
    ss << " }, cornerpos: { ";
    bAddComma = false;
    for (int i = 0; i < 8; i++)
    {
        if (bAddComma) ss << ", ";
        else bAddComma = true;
        ss << (int)m_cornerPos[i];
    }
    ss << " }, cornerrot: { ";
    bAddComma = false;
    for (int i = 0; i < 8; i++)
    {
        if (bAddComma) ss << ", ";
        else bAddComma = true;
        ss << (int)m_cornerRot[i];
    }
    ss << " }";

    return ss.str();
        
}

using namespace std;

int main(int argc, char* argv[])
{
    const unsigned char centerPos[] = { 5, 4, 3, 2, 1, 0 };
    const unsigned char cornerPos[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    const unsigned char cornerRot[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    Skewb skewb(centerPos, cornerPos, cornerRot);

uint64_t n = skewb.getStateNum();
    cout << "statestr: " << skewb.getStateStr() << endl;
    cout << "statenum: " << n << endl;

    Skewb skewb2(n);
    cout << endl << "statestr: " << skewb.getStateStr() << endl;

    return 0;
} 
