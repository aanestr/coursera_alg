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
    uint _opened_amount = 0;

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
        _opened_amount += 1;

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
        if(!isOpen(row, col))
            return false;

        return _qu.is_connected(calculate_index(row, col), 0);
    }

    // returns the number of open sites
    int numberOfOpenSites()
    {
       return _opened_amount;
    }

    bool percolates()
    {
        return _qu.is_connected(0, _n * _n + 1);
    }
};

void main_impl(int argc, const char * argv[])
{
    uint N = 20;
    uint T = 10;

    if(argc > 1)
        N = stoul(argv[1]);
    if(argc > 2)
        T = stoul(argv[2]);

    double fractions_sum = 0;
    vector<double> all_fractions;
    all_fractions.reserve(T);

    for (int i = 0; i < T; ++i)
    {
        Percolation p(N);
    
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, N - 1);
    
        while(!p.percolates())
        {
            uint row = dis(gen);
            uint col = dis(gen);
            
            if(!p.isOpen(row, col))
                p.open(row, col);
        }
        
        uint open_sites = p.numberOfOpenSites();

        double fraction = (double)open_sites / (double)(N * N);
    
        fractions_sum += fraction;
        all_fractions.push_back(fraction);
    }

    double x = (double)fractions_sum / (double)T;
    


    double ssum = 0.0;

    for(auto fraction : all_fractions)
    {
        auto c = (fraction - x) * (fraction - x);
        ssum += c;
    }

    double ss = ssum / (T - 1);
    double s = sqrt(ss);
    double t = sqrt(T);

    double tmp = ((1.96 * s) / t);
    double d1 = x - tmp;
    double d2 = x + tmp;

    cout << "mean   = " << x << endl;
    cout << "stddev = " << sqrt(ss) << endl;
    cout << "95% confidence interval = [" << d1 << ", " << d2 << "]" << endl;
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
