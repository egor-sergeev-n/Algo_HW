// Пятнашки

#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <stack>
#include <set>
#include <algorithm>
#include <sstream>
#include "assert.h"

static const int LINE_SIZE = 4;
static const int FIELD_SIZE = LINE_SIZE * LINE_SIZE;

using FieldArray = std::array<uint8_t, FIELD_SIZE>;

static const FieldArray EMPTY_FIELD = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const FieldArray GOAL_FIELD = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

class FieldState
{
public:
    FieldState() : heuristic(findHeuristic(EMPTY_FIELD)), weight(heuristic)
    {
        init(EMPTY_FIELD);
    }

    FieldState(const FieldArray &arr) : heuristic(findHeuristic(arr)), weight(heuristic)
    {
        init(arr);
    }

    FieldState(const FieldState &) = default;

    bool operator<(const FieldState &rhs) const
    {
        return arr_state < rhs.arr_state;
    }

    bool operator==(const FieldState &rhs) const
    {
        return arr_state == rhs.arr_state;
    }

    bool operator!=(const FieldState &rhs) const
    {
        return !operator==(rhs);
    }

    std::vector<FieldState> getNextVertices() const
    {
        std::vector<FieldState> res;

        if (zero_pos % LINE_SIZE != 0) // left
        {
            FieldState st(*this);
            st.swapZero(zero_pos - 1);
            res.push_back(st);
        }

        if ((zero_pos + 1) % LINE_SIZE != 0) // right
        {
            FieldState st(*this);
            st.swapZero(zero_pos + 1);
            res.push_back(st);
        }

        if (zero_pos >= LINE_SIZE) // up
        {
            FieldState st(*this);
            st.swapZero(zero_pos - LINE_SIZE);
            res.push_back(st);
        }

        if (zero_pos < (FIELD_SIZE - LINE_SIZE)) // down
        {
            FieldState st(*this);
            st.swapZero(zero_pos + LINE_SIZE);
            res.push_back(st);
        }

        return res;
    }

    bool isCorrectStart() const
    {
        int k = zero_pos / LINE_SIZE + 1;

        int N = 0;
        for (size_t i = 0; i < FIELD_SIZE; ++i)
        {
            int cur_num = arr_state[i];
            if (cur_num != 0)
                N += std::count_if(arr_state.begin(),
                                   arr_state.begin() + i,
                                   [cur_num](const int num_before)
                                   { return num_before > cur_num; });
        }

        return (N + k) % 2 == 0;
    }

    int getZeroPos() const
    {
        return zero_pos;
    }

    void print(const std::string &prefix = "") const
    {
        for (size_t i = 0; i < arr_state.size(); ++i)
        {
            if (i % LINE_SIZE == 0)
                std::cout << prefix;
            std::cout << (int)arr_state[i] << ' ';
            if ((i + 1) % LINE_SIZE == 0)
                std::cout << '\n';
        }
        std::cout << prefix << "Heuristic = " << heuristic << ", weight = " << weight << "\n\n";
    }

    void setDist(const int dist)
    {
        weight = heuristic + dist;
    }

    int getWieght() const
    {
        return weight;
    }

    bool isFinish() const
    {
        for (int i = 0; i < FIELD_SIZE; ++i)
            if (arr_state[i] != GOAL_FIELD[i])
                return false;
        return true;
    }

private:
    int findHeuristic(const FieldArray &arr)
    {
        int h = 0;

        for (int i = 0; i < FIELD_SIZE; ++i)
        {
            int x1 = i % LINE_SIZE;
            int y1 = i / LINE_SIZE;

            int x2, y2;
            if (arr[i] == 0)
            {
                x2 = 3;
                y2 = 3;
            }
            else
            {
                x2 = (arr[i] - 1) % LINE_SIZE;
                y2 = (arr[i] - 1) / LINE_SIZE;
            }

            h += std::abs(x1 - x2) + std::abs(y1 - y2);
        }

        return 5 * h;
    }

    void init(const FieldArray &arr)
    {
        for (size_t i = 0; i < arr.size(); ++i)
        {
            arr_state[i] = arr[i];
            if (arr_state[i] == 0)
                zero_pos = i;
        }
    }

    void swapZero(const int new_zero_pos)
    {
        std::swap(arr_state[zero_pos], arr_state[new_zero_pos]);
        zero_pos = new_zero_pos;
        heuristic = findHeuristic(arr_state);
    }

    FieldArray arr_state;
    int zero_pos, heuristic, weight;
};

struct FieldStateComparator
{
    bool operator()(const FieldState &lhs, const FieldState &rhs) const
    {
        if (lhs.getWieght() == rhs.getWieght())
            return lhs < rhs;
        return lhs.getWieght() < rhs.getWieght();
    }
};

void aStar(FieldState &start, std::ostream &out)
{
    std::map<FieldState, FieldState> parent;
    std::map<FieldState, int> dist;
    std::set<FieldState, FieldStateComparator> s;

    dist[start] = 0;
    start.setDist(0);
    s.insert(start);

    FieldState cur_state;

    while (!s.empty())
    {
        cur_state = *s.begin();
        s.erase(s.begin());

        if (cur_state.isFinish())
            break;

        int cur_dist = dist[cur_state];
        int new_dist = cur_dist + 1;

        for (FieldState &child : cur_state.getNextVertices())
        {
            if (dist.find(child) == dist.end()) // пришли первый раз
            {
                child.setDist(new_dist);
                dist[child] = new_dist;
                parent[child] = cur_state;
                s.insert(child);
            }
            else if (dist[child] > new_dist) // расстояние короче
            {
                s.erase(child);
                child.setDist(new_dist);
                dist[child] = new_dist;
                parent[child] = cur_state;
                s.insert(child);
            }
        }
    }

    std::string moves = "";
    while (cur_state != start)
    {
        int cur_zero_pos = cur_state.getZeroPos();
        cur_state = parent[cur_state];
        int parent_zero_pos = cur_state.getZeroPos();

        if (parent_zero_pos + 1 == cur_zero_pos)
            moves += 'L';
        else if (parent_zero_pos - 1 == cur_zero_pos)
            moves += 'R';
        else if (parent_zero_pos + LINE_SIZE == cur_zero_pos)
            moves += 'U';
        else
            moves += 'D';
    }

    std::reverse(moves.begin(), moves.end());
    out << moves.size() << '\n'
        << moves;
}

void processRequest(std::istream &in, std::ostream &out)
{
    FieldArray start_arr;
    for (size_t i = 0; i < FIELD_SIZE; ++i)
    {
        int num;
        in >> num;
        start_arr[i] = num;
    }

    FieldState start(start_arr);
    if (!start.isCorrectStart())
    {
        out << "-1";
        return;
    }

    aStar(start, out);
}

void testRequest()
{
    {
        FieldState goal_state(GOAL_FIELD);
        assert(goal_state.getWieght() == 0);
    }

    {
        std::stringstream in;
        in << "1 2 3 4 ";
        in << "5 6 7 8 ";
        in << "9 10 11 12 ";
        in << "13 15 14 0";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "-1");
    }

    {
        std::stringstream in;
        in << "1 2 3 4 ";
        in << "5 6 7 8 ";
        in << "9 10 11 12 ";
        in << "13 14 15 0";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "0\n");
    }

    {
        std::stringstream in;
        in << "1 2 3 4 ";
        in << "5 6 7 8 ";
        in << "9 10 11 0 ";
        in << "13 14 15 12";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "1\nU");
    }

    {
        std::stringstream in;
        in << "1 2 3 4 ";
        in << "5 6 7 8 ";
        in << "9 10 11 12 ";
        in << "13 14 0 15";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "1\nL");
    }

    {
        std::stringstream in;
        in << "1 2 3 4 ";
        in << "5 6 7 8 ";
        in << "9 10 0 12 ";
        in << "13 14 11 15";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "2\nUL");
    }

    {
        std::stringstream in;
        in << "1 2 3 4 ";
        in << "5 6 7 8 ";
        in << "9 0 10 12 ";
        in << "13 14 11 15";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "3\nLUL");
    }

    {
        std::stringstream in;
        in << "1 2 3 4 ";
        in << "5 6 7 8 ";
        in << "9 14 10 12 ";
        in << "13 0 11 15";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "4\nDLUL");
    }

    {
        std::stringstream in;
        in << "1 2 3 4 ";
        in << "5 6 7 8 ";
        in << "9 14 10 12 ";
        in << "13 11 0 15";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "5\nRDLUL");
    }

    {
        std::stringstream in;
        in << "1 2 3 0 ";
        in << "5 6 7 4 ";
        in << "9 10 11 8 ";
        in << "13 14 15 12";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "3\nUUU");
    }

    {
        std::stringstream in;
        in << "5 1 2 3 ";
        in << "9 6 7 4 ";
        in << "0 10 11 8 ";
        in << "13 14 15 12";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "8\nDDLLLUUU");
    }

    {
        std::stringstream in;
        in << "5 1 2 3 ";
        in << "9 6 7 4 ";
        in << "13 10 11 8 ";
        in << "14 15 12 0";
        std::stringstream out;
        processRequest(in, out);
        assert(out.str() == "12\nRRRDDDLLLUUU");
    }

    std::cout << "TESTS PASSED\n";
}

int main()
{
    testRequest();
    //processRequest(std::cin, std::cout);
    return 0;
}