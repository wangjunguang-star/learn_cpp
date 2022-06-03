#ifndef DATASET_H_
#define DATASET_H_

#include "utils.h"

namespace dataset {

class Dataset {
public:
    explicit Dataset(std::string file_path, uint32_t feature_num) 
        : m_file_path(file_path)
        , m_feature_num(feature_num)
        , m_class_num(2)
        , m_sample_num(100)  {
        m_file_path = file_path;
        m_feature_num = feature_num;
    }

    Dataset() = delete;

    bool load() {
        // data format : label \t float \t float \t float ...
        if(m_file_path.empty()) {
            std::cout << "file path empty" << "\n";
            return false;
        }

        std::string line("");
        std::vector<std::string> parts;
        std::ifstream is(m_file_path);
        if(is.is_open()) {
            std::vector<float> _x;
            while(getline(is, line)) {
                parts.clear();
                int sz = utils::split_string_n(line, '\t', parts);
                //std::cout << parts.size() << "  " << _x.size()  <<  "  "  << m_feature_num << " \n";
                assert(parts.size() == m_feature_num + 1);   // 通过parts.size()来遍历, 一定要确保不会越界访问
                m_y.push_back(atoi(parts[0].c_str()));
                _x.clear();
                for(int i=1; i<parts.size(); ++i) {
                    _x.push_back(atof(parts[i].c_str()));
                }
                m_X.push_back(_x);
            }
        } else {
            std::cout << "打开训练文件失败 \n" ;
            return false;
        }

        assert(m_X.size() == m_y.size());
        assert(m_X.size() > 100);
        assert(m_X[0].size() == m_feature_num);

        m_feature_num = get_feature_number();
        m_class_num = get_class_number();
        m_sample_num = get_sample_number();

        return true;
    }

    void get_train_test_dataset(std::vector<std::vector<float>>& train_X, std::vector<uint32_t>& train_y
                                , std::vector<std::vector<float>>& test_X, std::vector<uint32_t>& test_y
                                , float ratio=0.25) const;
    uint32_t get_sample_number() const;
    uint32_t get_class_number() const;
    uint32_t get_feature_number() const;
    void debug() {
        int debug_samples = 10;
        for(int i=0; i<debug_samples && i<m_X.size(); ++i) {
            const std::vector<float>& _x = m_X[i];
            for(auto it=_x.begin(); it!=_x.end(); ++it) {
                std::cout<< " "  << *it; 
            }
            std::cout <<" " <<  m_y[i] << "\n";
        } 
    
    }

private:
    std::string m_file_path;
    std::vector<std::vector<float>> m_X;
    std::vector<uint32_t> m_y;
    uint32_t m_feature_num;
    uint32_t m_class_num;
    uint32_t m_sample_num;

};

} // namespace dataset

#endif
