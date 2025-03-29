#include "pch.h"

#include <iostream>
#include <unistd.h>
#include <limits.h>

using namespace std;

struct QuickUnionImproved
{
    vector<size_t> _connections;
    vector<size_t> _sizes;
    map<size_t, string> _times;
    
    QuickUnionImproved(size_t max_elements)
    {
        _connections.resize(max_elements);
        _sizes.resize(max_elements);

        
        for(size_t i = 0; i < max_elements; ++i)
        {
            _sizes[i] = 1;
            _connections[i] = i;
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
    
    void unite(size_t p, size_t q, string timestamp)
    {
        size_t i = find_root(p);
        size_t j = find_root(q);
        
        if(i == j)
            return;
        
        if(_sizes[i] < _sizes[j])
        {
            _connections[i] = j;
            _times[j] = timestamp;
        }
        else
        {
            _connections[j] = i;

            if(_sizes[i] == _sizes[j])
                _sizes[i] += 1;

            _times[i] = timestamp;
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

    string timeof(size_t x)
    {
        auto r = find_root(x);
        return _times[r];
    }
};

auto read_people_file(filesystem::path p)
{
    size_t current_index = 0;
    map<string, size_t> resultMap;
    
    ifstream ifs {p};
    if(!ifs)
        throw runtime_error(string{"failed to open people file: "} + p.string());
    
    string ss;
    while(ifs >> ss)
    {
        resultMap[ss] = current_index++;
    }
    
    return resultMap;
}

auto create_network_connectivity(auto people2Id, auto friendship_file)
{
    ifstream ifs {friendship_file};
    if(!ifs)
    {
        throw runtime_error(string{"failed to open friendship file: "} + friendship_file.string());
    }

    string s1, s2;
    string date;
    string dash;

    QuickUnionImproved p_union(people2Id.size());
    
    while(ifs >> date >> dash >> s1 >> dash >> s2)
    {
        size_t p1 = people2Id[s1];
        size_t p2 = people2Id[s2];
     
        p_union.unite(p1, p2, date);
    }

    return p_union;
}

void main_impl(int argc, const char * argv[])
{
    if(argc < 3)
    {
        throw runtime_error("not enough arguments");
    }

    filesystem::path people_file {argv[1]};
    filesystem::path friendship_file = {argv[2]};

    auto people2Id = read_people_file(people_file);
    
    QuickUnionImproved network_connectivity = create_network_connectivity(people2Id, friendship_file);
    
    cout << network_connectivity.all_connected() << " at " << network_connectivity.timeof(0) << endl;
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
