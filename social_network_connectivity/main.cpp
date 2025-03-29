#include "pch.h"

#include <iostream>
#include <unistd.h>
#include <limits.h>

using namespace std;

struct QuickUnionImproved
{
    vector<size_t> _connections;
    vector<size_t> _sizes;
    
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
    
    void unite(size_t p, size_t q)
    {
        size_t i = find_root(p);
        size_t j = find_root(q);
        
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
};

map<string, size_t> read_people_file(filesystem::path p)
{
    size_t current_index = 0;
    map<string, size_t> resultMap;
    
    ifstream ifs {p};
    if(!ifs)
        throw runtime_error(string{"failed to open people file: "} + p.string());
    
    string s1;
    string s2;
    while(ifs >> s1 >> s2)
    {
        resultMap[s1+s2] = current_index++;
    }
    
    return resultMap;
}

void main_impl(int argc, const char * argv[])
{
    //        if(argc < 3)
    //        {
    //            throw runtime_error("now enough arguments");
    //        }
    //        filesystem::path p {argv[1]};
    
    filesystem::path dir {"/Users/void/develop/coursera_alg/social-network-connectivity/social-network-connectivity"};
    
    filesystem::path people_file = dir / "test_people.txt";
    filesystem::path friendship_file = dir / "test_friendship.txt";
    
    auto people2id = read_people_file(people_file);
    
    ifstream ifs {friendship_file};
    if(!ifs)
        throw runtime_error(string{"failed to open friendship file: "} + friendship_file.string());

    
    string s1, s2, s3, s4;
    string date, time;
    string dash;

    QuickUnionImproved p_union(people2id.size());
    
    while(ifs >> date >> time >> dash >> s1 >> s2 >> dash >> s3 >> s4)
    {
        size_t p1 = people2id[s1+s2];
        size_t p2 = people2id[s3+s4];
     
        p_union.unite(p1, p2);
    }
    
    cout << p_union.all_connected() << endl;
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
