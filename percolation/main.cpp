#include "pch.h"

using namespace std;

class QuickUnionImproved
{
public:
    void Print(const uint n)
    {
        for(uint i = 0; i < _connections.size() - 2; ++i)
        {
            if(i != 0 && i % n == 0)
                cout << endl;
            cout << _connections[i + 1] << " ";
        }
        cout << endl;
    }

    QuickUnionImproved(uint max_elements)
    {
        _connections.resize(max_elements);
        _sizes.resize(max_elements);

        for(uint i = 0; i < max_elements; ++i)
        {
            _sizes[i] = 1;
            _connections[i] = i;
        }
    }
    
    uint find_root(uint x)
    {
        while(_connections[x] != x)
        {
            x = _connections[x];
        }
        
        return x;
    }
    
    void unite(uint p, uint q)
    {
        uint i = find_root(p);
        uint j = find_root(q);
        
        if(i == j)
            return;
        
        if(_sizes[i] < _sizes[j])
        {
            _connections[i] = j;
        }
        else
        {
            _connections[j] = i;

            if(_sizes[i] == _sizes[j])
                _sizes[i] += 1;
        }
    }
    
    bool is_connected(uint p, uint q)
    {
        return find_root(p) == find_root(q);
    }
    
    bool all_connected()
    {
        auto root = find_root(0);
        
        for(uint i = 1; i < _connections.size(); ++i)
        {
            auto r = find_root(i);
            if(r != root)
                return false;
        }
        
        return true;
    }

private:
    vector<uint> _connections;
    vector<uint> _sizes;
};

struct Percolation 
{
    QuickUnionImproved _qu;
    vector<vector<uint>> _opened;
    const uint _n;

    void Print()
    {
        ofstream ofs("data.csv");
        if(!ofs)
            throw runtime_error("can not open output file");
        
        for(uint i = 0; i < _opened.size(); ++i)
        {
            for(uint j = 0; j < _opened[i].size(); ++j)
            {
                ofs << _opened[i][j] << ";";
            }
            ofs << endl;
        }
    }

    Percolation(int n)
    : _n(n)
    , _qu(n * n + 2)
    , _opened(n, vector<uint>(n, 0))
    {
    }

    uint calculate_index(uint row, uint col)
    {
        return row * _n + col + 1;
    }
    
    // opens the site (row, col) if it is not open already
    void open(int row, int col)
    {
        _opened[row][col] = 1;

        uint v1 = calculate_index(row, col);

        //update left
        if(col != 0 && _opened[row][col - 1] == 1)
        {
            uint v2 = calculate_index(row, col - 1);
            _qu.unite(v1, v2);        
        }

        //update right
        if((col != _n -1) && _opened[row][col + 1] == 1)
        {
            uint v2 = calculate_index(row, col + 1);
            _qu.unite(v1, v2);
        }

        //update top
        if(row != 0 && _opened[row - 1][col] == 1)
        {   
            uint v2 = calculate_index(row - 1, col);
            _qu.unite(v1, v2);
        }

        //update bottom
        if((row != _n - 1) && _opened[row + 1][col] == 1)
        {
            uint v2 = calculate_index(row + 1, col);
            _qu.unite(v1, v2);
        }

        //connect with virtual sites
        if(row == 0)
            _qu.unite(v1, 0);
        if(row == _n - 1)
            _qu.unite(v1, _n * _n + 1);
    }

    bool isOpen(int row, int col)
    {
        return _opened[row][col] == 1;
    }

    // is the site (row, col) full?
    bool isFull(int row, int col)
    {
        return !isOpen(row, col);
    }

    // returns the number of open sites
    int numberOfOpenSites()
    {
        uint amount = 0;
        for(uint i = 0; i < _opened.size(); ++i)
        {
            for(uint j = 0; j < _opened[i].size(); ++j)
            {
                if(_opened[i][j] == 1)
                    amount += 1;
            }
        }
        return amount;
    }

    bool percolates()
    {
        return _qu.is_connected(0, _n * _n + 1);
    }
};

void main_impl(int argc, const char * argv[])
{
    uint N = 20;

    Percolation p(N);
    
    std::random_device rd;  // Obtain a seed from the hardware
    std::mt19937 gen(rd()); // Initialize the random engine with the seed
    std::uniform_int_distribution<> dis(0, 400);

    for(uint i = 0; i < N; ++i)
    {
        for(uint j = 0; j < N; ++j)
        {
            int random_value = dis(gen);

            if(random_value < 220)
                p.open(i, j);
        }
    }
        
    p.Print();

    cout << p.percolates() << endl;
}

int main(int argc, const char * argv[])
{
    try
    {
        main_impl(argc, argv);
    }
    catch(exception &ex)
    {
        cout << ex.what() << endl;
        return -1;
    }
    
    return 0;
}
