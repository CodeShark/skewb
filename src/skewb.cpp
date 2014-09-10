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
    // Move names
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

    // Center names
    //
    // Bottom: 0 (Orange)
    // Top: 1 (Red)
    // Back left: 2 (Yellow)
    // Front right: 3 (White)
    // Back right: 4 (Green)
    // Front left: 5 (Blue)

    // Corner names (viewed from top)
    //
    // Upper
    //     4
    //   5   7
    //     6
    //
    // Lower
    //     0
    //   1   3
    //     2

    enum move_t
    {
        UP_C    =  (1 << 1),
        UP_CC,
        DOWN_C  =  (1 << 2),
        DOWN_CC,
        LEFT_C  =  (1 << 3),
        LEFT_CC,
        RIGHT_C =  (1 << 4),
        RIGHT_CC
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
        m_cornerRot[7 - i] = stateNum % 3;
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
        m_centerPos[6 - i] = stateNum % i;
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

    unsigned char cornerPos[7];
    memcpy(cornerPos, m_cornerPos, 7);
    for (int i = 0; i < 6; i++)
        for (int j = i + 1; j < 7; j++)
            if (cornerPos[j] > cornerPos[i]) cornerPos[j]--;
    for (int i = 0; i < 7; i++)
        n = n * (8 - i) + cornerPos[i];

    for (int i = 0; i < 8; i++)
        n = n * 3 + m_cornerRot[i];

    return n;
}

void Skewb::makeMove(move_t move)
{
    unsigned char temp;

    switch (move)
    {
    case UP_C:
        temp = m_centerPos[1];
        m_centerPos[1] = m_centerPos[5];
        m_centerPos[5] = m_centerPos[3];
        m_centerPos[3] = temp;

        temp = m_cornerPos[2];
        m_cornerPos[2] = m_cornerPos[7];
        m_cornerPos[7] = m_cornerPos[5];
        m_cornerPos[5] = temp;

        m_cornerRot[6] = (m_cornerRot[6] + 1) % 3;
        m_cornerRot[2] = (m_cornerRot[2] + 2) % 3;
        m_cornerRot[4] = (m_cornerRot[4] + 2) % 3;
        m_cornerRot[7] = (m_cornerRot[7] + 2) % 3;
        break; 

    case UP_CC:
        temp = m_centerPos[1];
        m_centerPos[1] = m_centerPos[3];
        m_centerPos[3] = m_centerPos[5];
        m_centerPos[5] = temp;

        temp = m_cornerPos[2];
        m_cornerPos[2] = m_cornerPos[5];
        m_cornerPos[5] = m_cornerPos[7];
        m_cornerPos[7] = temp;

        m_cornerRot[6] = (m_cornerRot[6] + 2) % 3;
        m_cornerRot[2] = (m_cornerRot[2] + 1) % 3;
        m_cornerRot[4] = (m_cornerRot[4] + 1) % 3;
        m_cornerRot[7] = (m_cornerRot[7] + 1) % 3;
        break; 

    case DOWN_C:
        temp = m_centerPos[0];
        m_centerPos[0] = m_centerPos[3];
        m_centerPos[3] = m_centerPos[5];
        m_centerPos[5] = temp;

        temp = m_cornerPos[1];
        m_cornerPos[1] = m_cornerPos[3];
        m_cornerPos[3] = m_cornerPos[6];
        m_cornerPos[6] = temp;

        m_cornerRot[2] = (m_cornerRot[2] + 1) % 3;
        m_cornerRot[1] = (m_cornerRot[1] + 2) % 3;
        m_cornerRot[3] = (m_cornerRot[3] + 2) % 3;
        m_cornerRot[6] = (m_cornerRot[6] + 2) % 3;
        break; 

    case DOWN_CC:
        temp = m_centerPos[0];
        m_centerPos[0] = m_centerPos[5];
        m_centerPos[5] = m_centerPos[3];
        m_centerPos[3] = temp;

        temp = m_cornerPos[1];
        m_cornerPos[1] = m_cornerPos[6];
        m_cornerPos[6] = m_cornerPos[3];
        m_cornerPos[3] = temp;

        m_cornerRot[2] = (m_cornerRot[2] + 2) % 3;
        m_cornerRot[1] = (m_cornerRot[1] + 1) % 3;
        m_cornerRot[3] = (m_cornerRot[3] + 1) % 3;
        m_cornerRot[6] = (m_cornerRot[6] + 1) % 3;
        break; 

    case LEFT_C:
        temp = m_centerPos[1];
        m_centerPos[1] = m_centerPos[2];
        m_centerPos[2] = m_centerPos[5];
        m_centerPos[5] = temp;

        temp = m_cornerPos[1];
        m_cornerPos[1] = m_cornerPos[6];
        m_cornerPos[6] = m_cornerPos[4];
        m_cornerPos[4] = temp;

        m_cornerRot[5] = (m_cornerRot[5] + 1) % 3;
        m_cornerRot[1] = (m_cornerRot[1] + 2) % 3;
        m_cornerRot[4] = (m_cornerRot[4] + 2) % 3;
        m_cornerRot[6] = (m_cornerRot[6] + 2) % 3;
        break; 

    case LEFT_CC:
        temp = m_centerPos[1];
        m_centerPos[1] = m_centerPos[5];
        m_centerPos[5] = m_centerPos[2];
        m_centerPos[2] = temp;

        temp = m_cornerPos[1];
        m_cornerPos[1] = m_cornerPos[4];
        m_cornerPos[4] = m_cornerPos[6];
        m_cornerPos[6] = temp;

        m_cornerRot[5] = (m_cornerRot[5] + 2) % 3;
        m_cornerRot[1] = (m_cornerRot[1] + 1) % 3;
        m_cornerRot[4] = (m_cornerRot[4] + 1) % 3;
        m_cornerRot[6] = (m_cornerRot[6] + 1) % 3;
        break; 

    case RIGHT_C:
        temp = m_centerPos[1];
        m_centerPos[1] = m_centerPos[3];
        m_centerPos[3] = m_centerPos[4];
        m_centerPos[4] = temp;

        temp = m_cornerPos[3];
        m_cornerPos[3] = m_cornerPos[4];
        m_cornerPos[4] = m_cornerPos[6];
        m_cornerPos[6] = temp;

        m_cornerRot[7] = (m_cornerRot[7] + 1) % 3;
        m_cornerRot[3] = (m_cornerRot[3] + 2) % 3;
        m_cornerRot[4] = (m_cornerRot[4] + 2) % 3;
        m_cornerRot[6] = (m_cornerRot[6] + 2) % 3;
        break; 

    case RIGHT_CC:
        temp = m_centerPos[1];
        m_centerPos[1] = m_centerPos[4];
        m_centerPos[4] = m_centerPos[3];
        m_centerPos[3] = temp;

        temp = m_cornerPos[3];
        m_cornerPos[3] = m_cornerPos[6];
        m_cornerPos[6] = m_cornerPos[4];
        m_cornerPos[4] = temp;

        m_cornerRot[7] = (m_cornerRot[7] + 2) % 3;
        m_cornerRot[3] = (m_cornerRot[3] + 1) % 3;
        m_cornerRot[4] = (m_cornerRot[4] + 1) % 3;
        m_cornerRot[6] = (m_cornerRot[6] + 1) % 3;
        break; 
    }
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
    ss << "centerPos: { ";
    bool bAddComma = false;
    for (int i = 0; i < 6; i++)
    {
        if (bAddComma) ss << ", ";
        else bAddComma = true;
        ss << (int)m_centerPos[i];
    }
    ss << " }, cornerPos: { ";
    bAddComma = false;
    for (int i = 0; i < 8; i++)
    {
        if (bAddComma) ss << ", ";
        else bAddComma = true;
        ss << (int)m_cornerPos[i];
    }
    ss << " }, cornerRot: { ";
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
    const unsigned char cornerPos[] = { 0, 1, 2, 3, 5, 7, 6, 4 };
    const unsigned char cornerRot[] = { 0, 2, 0, 0, 1, 0, 0, 0 };
    Skewb skewb(centerPos, cornerPos, cornerRot);

    uint64_t n = skewb.getStateNum();
    cout << "statestr: " << skewb.getStateStr() << endl;
    cout << "statenum: " << n << endl;

    Skewb skewb2(n);
    cout << endl << "statestr: " << skewb2.getStateStr() << endl;

    return 0;
} 
