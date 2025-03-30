#include "pch.h"

#include <iostream>
#include <unistd.h>
#include <limits.h>

using namespace std;

struct QuickUnionImproved
{
    vector<size_t> _connections;
    vector<size_t> _sizes;
    vector<size_t> _max;

    QuickUnionImproved(size_t max_elements)
    {
        _connections.resize(max_elements);
        _sizes.resize(max_elements);
        _max.resize(max_elements);
        
        for(size_t i = 0; i < max_elements; ++i)
        {
            _sizes[i] = 1;
            _connections[i] = i;
            _max[i] = i;
        }
    }
    
    size_t find_root(size_t x)
    {
        while(_connections[x] != x)
        {
            x = _connections[x];
        }
        
        return x;
    }
    
    void unite(size_t p, size_t q)
    {
        size_t i = find_root(p);
        size_t j = find_root(q);
        
        if(i == j)
            return;
        
        auto maxValue = std::max(_max[i], _max[j]);

        if(_sizes[i] < _sizes[j])
        {
            _connections[i] = j;
            _max[j] = maxValue;
        }
        else
        {
            _connections[j] = i;
            _max[i] = maxValue;

            if(_sizes[i] == _sizes[j])
                _sizes[i] += 1;
        }
    }
    
    bool is_connected(size_t p, size_t q)
    {
        return find_root(p) == find_root(q);
    }
    
    bool all_connected()
    {
        auto root = find_root(0);
        
        for(size_t i = 1; i < _connections.size(); ++i)
        {
            auto r = find_root(i);
            if(r != root)
                return false;
        }
        
        return true;
    }

    size_t find(size_t x)
    {
        return _max[find_root(x)];
    }
};

void main_impl(int argc, const char * argv[])
{
    QuickUnionImproved q(10);

    q.unite(4, 3);
    q.unite(3, 8);
    q.unite(6, 5);
    q.unite(9, 4);
    q.unite(2, 1);
    q.unite(5, 0);
    q.unite(7, 2);
    q.unite(6, 1);

    cout << q.find(0) << endl;
    cout << q.find(2) << endl;
    cout << q.find(5) << endl;
    cout << q.find(1) << endl;
    cout << q.find(7) << endl;
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
