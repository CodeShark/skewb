#include <iostream>

class Skewb
{
public:
    Skewb();

private:
    int m_centerPos[6];
    int m_cornerPos[8];
    int m_cornerRot[8];
};

Skewb::Skewb()
{
    for (int i = 0; i < 6; i++) m_centerPos[i] = i;
    for (int i = 0; i < 8; i++) m_cornerPos[i] = i;
    for (int i = 0; i < 8; i++) m_cornerRot[i] = 0;
}

using namespace std;

int main(int argc, char* argv[])
{
    Skewb skewb;
    cout << "boo!" << endl;
    return 0;
} 
