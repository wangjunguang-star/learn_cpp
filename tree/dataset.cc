#include "dataset.h"
#include "tree.h"
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




    uint32_t Dataset::get_sample_number() const {
        return m_y.size();
    }

    uint32_t Dataset::get_class_number() const {
        std::map<uint32_t, uint32_t> y_map;
        reduce_y_by_label(m_y, y_map);
        return y_map.size();
    }

    uint32_t Dataset::get_feature_number() const {
        return m_X[0].size();
    }
    
    void Dataset::get_train_test_dataset(std::vector<std::vector<float>>& train_X, std::vector<uint32_t>& train_y
                                , std::vector<std::vector<float>>& test_X, std::vector<uint32_t>& test_y
                                , float ratio) const {
        
        uint32_t sample_num = get_sample_number();
        uint32_t train_num = int(sample_num*(1-ratio));
        uint32_t test_num = sample_num - train_num;
        assert(test_num > 0);

        train_X.assign(m_X.begin(), m_X.begin()+train_num);
        train_y.assign(m_y.begin(), m_y.begin()+train_num);

        test_X.assign(m_X.begin()+train_num, m_X.end());
        test_y.assign(m_y.begin()+train_num, m_y.end());                                     
    }
    

