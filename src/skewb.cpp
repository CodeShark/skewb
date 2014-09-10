#include <iostream>
#include <vector>

const int FACTORIAL[] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320 };

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

    // C is clockwise and CC is counterclockwise

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

    Skewb() { init(); }
    Skewb(uint64_t stateNum) { setStateNum(stateNum); }

    void init();
    void setStateNum(uint64_t stateNum);
    uint64_t getStateNum() const;

    void makeMove(move_t move);
    const std::vector<move_t> getMoveSeq() const { return m_moveSeq; }

    bool isSolved() const;

private:
    int m_centerPos[6];
    int m_cornerPos[8];
    int m_cornerRot[8];
    std::vector<move_t> m_moveSeq; 
};

void Skewb::init()
{
    for (int i = 0; i < 6; i++) m_centerPos[i] = i;
    for (int i = 0; i < 8; i++) m_cornerPos[i] = i;
    for (int i = 0; i < 8; i++) m_cornerRot[i] = 0;
}

void setStateNum(uint64_t stateNum)
{
}

uint64_t Skewb::getStateNum() const
{
    uint64_t stateNum = 0;
    for (int i = 0; i < 6; i++)
    {
        stateNum += 
    }    
}

void Skewb::makeMove(move_t move)
{
}

bool Skewb::isSolved() const
{
}

using namespace std;

int main(int argc, char* argv[])
{
    Skewb skewb;
    cout << "boo!" << endl;
    return 0;
} 
