#include <iostream>
#include <vector>
#include <map>
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
        IDENTITY = 0,
        UP_C    =  (1 << 1),
        UP_CC,
        DOWN_C  =  (1 << 2),
        DOWN_CC,
        LEFT_C  =  (1 << 3),
        LEFT_CC,
        RIGHT_C =  (1 << 4),
        RIGHT_CC
    };

    static const char* getMoveSymbol(move_t move)
    {
        switch (move)
        {
        case IDENTITY:  return "I";
        case UP_C:      return "U";
        case UP_CC:     return "U'";
        case DOWN_C:    return "D";
        case DOWN_CC:   return "D'";
        case LEFT_C:    return "L";
        case LEFT_CC:   return "L'";
        case RIGHT_C:   return "R";
        case RIGHT_CC:  return "R'";
        }
    }

    static move_t getInverse(move_t move)
    {
        switch (move)
        {
        case IDENTITY:  return IDENTITY;
        case UP_C:      return UP_CC;
        case UP_CC:     return UP_C;
        case DOWN_C:    return DOWN_CC;
        case DOWN_CC:   return DOWN_C;
        case LEFT_C:    return LEFT_CC;
        case LEFT_CC:   return LEFT_C;
        case RIGHT_C:   return RIGHT_CC;
        case RIGHT_CC:  return RIGHT_C;
        }
    }

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
        for (int j = i + 1; j < 6; j++)
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
    case IDENTITY:
        break;

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
        m_cornerRot[2] = (m_cornerRot[7] + 2) % 3;
        m_cornerRot[5] = (m_cornerRot[2] + 2) % 3;
        m_cornerRot[7] = (m_cornerRot[5] + 2) % 3;
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
        m_cornerRot[2] = (m_cornerRot[5] + 1) % 3;
        m_cornerRot[5] = (m_cornerRot[7] + 1) % 3;
        m_cornerRot[7] = (m_cornerRot[2] + 1) % 3;
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
        m_cornerRot[1] = (m_cornerRot[3] + 2) % 3;
        m_cornerRot[3] = (m_cornerRot[6] + 2) % 3;
        m_cornerRot[6] = (m_cornerRot[1] + 2) % 3;
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
        m_cornerRot[1] = (m_cornerRot[6] + 1) % 3;
        m_cornerRot[3] = (m_cornerRot[1] + 1) % 3;
        m_cornerRot[6] = (m_cornerRot[3] + 1) % 3;
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
        m_cornerRot[1] = (m_cornerRot[6] + 2) % 3;
        m_cornerRot[4] = (m_cornerRot[1] + 2) % 3;
        m_cornerRot[6] = (m_cornerRot[4] + 2) % 3;
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
        m_cornerRot[1] = (m_cornerRot[4] + 1) % 3;
        m_cornerRot[4] = (m_cornerRot[6] + 1) % 3;
        m_cornerRot[6] = (m_cornerRot[1] + 1) % 3;
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
        m_cornerRot[3] = (m_cornerRot[4] + 2) % 3;
        m_cornerRot[4] = (m_cornerRot[6] + 2) % 3;
        m_cornerRot[6] = (m_cornerRot[3] + 2) % 3;
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
        m_cornerRot[3] = (m_cornerRot[6] + 1) % 3;
        m_cornerRot[4] = (m_cornerRot[3] + 1) % 3;
        m_cornerRot[6] = (m_cornerRot[4] + 1) % 3;
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


class TableEntry
{
public:
    TableEntry(uint64_t prevState, int distance, Skewb::move_t move)
        : m_prevState(prevState), m_distance(distance), m_move(move) { }

    uint64_t getPrevState() const { return m_prevState; }
    int getDistance() const { return m_distance; }
    Skewb::move_t getMove() const { return m_move; }

private:
    uint64_t m_prevState;
    int m_distance;
    Skewb::move_t m_move;
};

typedef std::pair<uint64_t, TableEntry> TablePair;
typedef std::pair<uint64_t, Skewb::move_t> LastState;

class SolutionTable
{
public:
    SolutionTable();

    bool addEntry(uint64_t prevState, uint64_t newState, Skewb::move_t move);
    uint64_t size() const { return m_table.size(); }

    std::vector<Skewb::move_t> getSolution(uint64_t state) const;
    std::string getSolutionStr(uint64_t state) const;
    int getMaxDistance() const { return m_maxDistance; }

    void generate();

private:
    std::map<uint64_t, TableEntry> m_table;
    int m_maxDistance;

    void pushNextState(const LastState& state, Skewb::move_t move, std::vector<LastState>& nextStates);
};

SolutionTable::SolutionTable() : m_maxDistance(0)
{
    m_table.insert(TablePair(0, TableEntry(0, 0, Skewb::IDENTITY)));
}

bool SolutionTable::addEntry(uint64_t prevState, uint64_t newState, Skewb::move_t move)
{
    if (m_table.count(newState)) return false;

    const auto& it = m_table.find(prevState);
    if (it == m_table.end()) throw std::runtime_error("Previous entry not found.");

    int distance = it->second.getDistance() + 1;
    if (distance > m_maxDistance) { m_maxDistance = distance; }
    m_table.insert(TablePair(newState, TableEntry(prevState, distance, move))); 

    return true;
}

std::vector<Skewb::move_t> SolutionTable::getSolution(uint64_t state) const
{
    auto it = m_table.find(state);
    if (it == m_table.end()) throw std::runtime_error("State not found.");

    std::vector<Skewb::move_t> solution;
    while (it->second.getDistance() > 0)
    {
        solution.push_back(Skewb::getInverse(it->second.getMove()));
        it = m_table.find(it->second.getPrevState());
        if (it == m_table.end()) throw std::runtime_error("Solution is missing.");
    }

    return solution;
}

std::string SolutionTable::getSolutionStr(uint64_t state) const
{
    std::vector<Skewb::move_t> solution = getSolution(state);

    std::stringstream ss;
    for (auto& move: solution)
    {
        ss << Skewb::getMoveSymbol(move) << " ";
    }
    return ss.str();
}

void SolutionTable::pushNextState(const LastState& state, Skewb::move_t move, std::vector<LastState>& nextStates)
{
    if (!((state.second & move) >> 1))
    {
        Skewb skewb(state.first);
        skewb.makeMove(move);
        uint64_t stateNum = skewb.getStateNum();
        if (addEntry(state.first, stateNum, move))
        {
//            std::cout << "Added " << stateNum << std::endl;
            nextStates.push_back(LastState(stateNum, move));
        }
    }
}

void SolutionTable::generate()
{
    int distance = 0;
    std::vector<LastState> lastStates;
    std::vector<LastState> nextStates;
    lastStates.push_back(LastState(0, Skewb::IDENTITY));

    while (!lastStates.empty())
    {
        std::cout << "Processing " << lastStates.size() << " states with distance " << distance << "..." << std::endl;

        distance++;
        for (auto& state: lastStates)
        {
            pushNextState(state, Skewb::UP_C, nextStates);
            pushNextState(state, Skewb::UP_CC, nextStates);
            pushNextState(state, Skewb::DOWN_C, nextStates);
            pushNextState(state, Skewb::DOWN_CC, nextStates);
            pushNextState(state, Skewb::LEFT_C, nextStates);
            pushNextState(state, Skewb::LEFT_CC, nextStates);
            pushNextState(state, Skewb::RIGHT_C, nextStates);
            pushNextState(state, Skewb::RIGHT_CC, nextStates);
        }
        lastStates = nextStates;
        nextStates.clear();
    }
}

using namespace std;

int main(int argc, char* argv[])
{/*
    const unsigned char centerPos[] = { 5, 4, 3, 2, 1, 0 };
    const unsigned char cornerPos[] = { 0, 1, 2, 3, 5, 7, 6, 4 };
    const unsigned char cornerRot[] = { 0, 2, 0, 0, 1, 0, 0, 0 };
    Skewb skewb(centerPos, cornerPos, cornerRot);

    uint64_t n = skewb.getStateNum();
    cout << "statestr: " << skewb.getStateStr() << endl;
    cout << "statenum: " << n << endl;

    Skewb skewb2(n);
    cout << endl << "statestr: " << skewb2.getStateStr() << endl;
*/

    try
    {
        cout << "Generating solution table..." << endl;
        SolutionTable solutions;
        solutions.generate();

        // Bottom: 0 (Orange)
        // Top: 1 (Red)
        // Back left: 2 (Yellow)
        // Front right: 3 (White)
        // Back right: 4 (Green)
        // Front left: 5 (Blue)

        const unsigned char centerPos[] = { 0, 4, 2, 1, 3, 5 };
        const unsigned char cornerPos[] = { 0, 3, 2, 1, 6, 5, 4, 7 };
        const unsigned char cornerRot[] = { 0, 1, 2, 1, 1, 2, 0, 2 };

/*
        const unsigned char centerPos[] = { 0, 5, 2, 1, 4, 3 };
        const unsigned char cornerPos[] = { 0, 1, 7, 3, 4, 2, 6, 5 };
        const unsigned char cornerRot[] = { 0, 0, 2, 0, 0, 2, 1, 2 };
*/
        Skewb skewb(centerPos, cornerPos, cornerRot);
/*
        Skewb skewb;
        skewb.makeMove(Skewb::UP_C);
        skewb.makeMove(Skewb::UP_C);
        skewb.makeMove(Skewb::LEFT_C);
        skewb.makeMove(Skewb::DOWN_C);
        skewb.makeMove(Skewb::LEFT_C);
        skewb.makeMove(Skewb::UP_C);
        skewb.makeMove(Skewb::RIGHT_CC);
*/
        uint64_t stateNum = skewb.getStateNum();

        cout << "Solving state " << stateNum << "..." << endl;
        cout << "Solution: " << solutions.getSolutionStr(stateNum) << endl;
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << endl;
        return -1;
    } 
    
    return 0;
} 
