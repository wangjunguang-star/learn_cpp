#include "utils.h"

namespace utiles{

void split_string(std::string& line, char delimiter, std::vector<std::string>* r)
{
    int begin = 0;
    for(int i = 0; i < line.size(); ++i)
    {
        if(line[i] == delimiter)
        {
            (*r).push_back(line.substr(begin, i - begin));
            begin = i + 1;
        }
    }
    if(begin < line.size())
    {
        (*r).push_back(line.substr(begin, line.size() - begin));
    }
}

void split_string(std::string& line, char delimiter, std::vector<std::string>& r)
{
    int begin = 0;
    for(int i = 0; i < line.size(); ++i)
    {
        if(line[i] == delimiter)
        {
            r.push_back(line.substr(begin, i - begin));
            begin = i + 1;
        }
    }
    if(begin < line.size())
    {
        r.push_back(line.substr(begin, line.size() - begin));
    }
}


int split_string_n(std::string& line, char delimiter, std::vector<std::string>& r)
{
    int num = 0;
    int begin = 0;
    for(int i = 0; i < line.size(); ++i)
    {
        if(line[i] == delimiter)
        {
            r.push_back(line.substr(begin, i - begin));
            begin = i + 1;
            num += 1;
        }
    }
    if(begin < line.size())
    {
        r.push_back(line.substr(begin, line.size() - begin));
        num += 1;
    }
    
    return num;
}

void reduce_y_by_label(const std::vector<uint32_t>& y, std::map<uint32_t, uint32_t>& y_map) {
    y_map.clear();
    std::vector<uint32_t>::const_iterator it = y.begin();
    for(; it!=y.end(); ++it) {
        if(y_map.find(*it) == y_map.end()) {
            y_map.insert(std::make_pair(*it, 1));
        } else {
            uint32_t& val = y_map[*it];
            val += 1;
        }
    }
}

float compute_entropy(const std::vector<uint32_t>& y) {
    float entropy = 0.0;
    uint32_t y_count = y.size();
    
    if(y_count <= 1) {
        return entropy;
    }

    std::map<uint32_t, uint32_t> y_map;
    reduce_y_by_label(y, y_map);

    for(auto it=y_map.begin(); it!=y_map.end(); ++it) {
        float p = it->second * 1.0 / y_count;
        entropy += -1 * p * log(p);  // 需要考虑p很小导致的计算问题吗？
    }
    
    return entropy;
}

} // namespace utiles

