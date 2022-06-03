#include <random>
#include <algorithm>
#include "tree.h"


namespace tree {

BaseNode* Tree::_traverse(BaseNode* root, const std::vector<float>& X) const {
    // std::cerr<< " root " << root << "  " << root->get_type() << "\n";

    if(root == NULL) return NULL;
    if(root->get_type() == "leaf") return root;

    uint32_t _split_index = root->get_split_index();
    float _split_threshold = root->get_split_threshold();
    assert(_split_index >=0 && _split_index < m_feature_num);
    assert(X.size() == m_feature_num);

    if(X[_split_index] < _split_threshold) {
        // std::cout << " left " << "\n";
        return _traverse(root->get_left(), X);
    } else {
        // std::cout << " right " << "\n";
        return _traverse(root->get_right(), X);
    }

    return NULL;
}

void Tree::_col_select(uint32_t feature_number, std::vector<uint32_t>& index) {
    static bool is_init = false;
    if(!is_init) {
        for(uint32_t i=0; i<feature_number; ++i) {
            m_features.push_back(i);
        }
    }
    is_init = true;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(m_features.begin(), m_features.end(), rng);
    index.assign(m_features.begin(), m_features.begin()+m_col_sample_feature_num );
}

// TODO X应该传递索引，样本应该有一个单独的类来进行处理
void Tree::_segment(const std::vector<std::vector<float>>& X
        , const std::vector<uint32_t>& y
        , const std::vector<uint32_t>& split_indexs
        , SplitInfo& split_info
        , std::vector<std::vector<float>>& x_left
        , std::vector<std::vector<float>>& x_right
        , std::vector<uint32_t>& y_left
        , std::vector<uint32_t>& y_right) {

    // std::cout << "         SEGMENT ...   \n";
    // std::cout << " all count is " << X.size() << " " << y.size() << "\n";
    // std::cout << " left count is " << x_left.size() << " " << y_left.size() << "\n";
    // std::cout << " left count is " << x_right.size() << " " << y_right.size() << "\n";

    float best_gain = NAGETIVE_INFINIT;
    // std::cout << " init best_gain = " << best_gain << "\n";
    SplitInfo sinfo;
    std::vector<float> _x;
    std::set<float> _s;
    for(int i=0; i<split_indexs.size(); ++i) {
        _x.clear();
        _s.clear();
        
        for(int k=0; k<X.size(); ++k) {
            _x.push_back(X[k][split_indexs[i]]);
            _s.insert(X[k][split_indexs[i]]);   // std::set 默认是排序的
        }
        // std::cout << " 去重前后的大小 " << _x.size() << " | " << _s.size() << "\n";

        for(auto iter=_s.begin(); iter!=_s.end(); ++iter) {
            auto iter_next = std::next(iter);
            if(iter_next == _s.end()) break;
            // std::cout << " i + (i+1) = " << *iter << "  "  << *iter_next << "\n";
            float _mid_split = 0.5*( *iter + *iter_next);
            float _gain = _impurity_gain(_x, y, _mid_split);
            // std::cout << "gain info : " << _mid_split << "  " << _gain << "  " << best_gain << "\n";
            if(_gain > best_gain) {
                split_info.split_idx = split_indexs[i];
                split_info.split_threshold = _mid_split;
                best_gain = _gain;
            }
        }
    }

    // std::cout << " final best split index and  gain = " << split_info.split_idx << "  " << best_gain << "\n";
    
    // 确定好分割点后对样本进行划分
    float _split_thres = split_info.split_threshold;
    for(int i=0; i < X.size(); ++i) {
        if(X[i][split_info.split_idx] <= _split_thres) {
            x_left.push_back(X[i]);
            y_left.push_back(y[i]);
        } else {
            x_right.push_back(X[i]);
            y_right.push_back(y[i]);
        }
    }
    // std::cout << " CCCCC  " << y_left.size() << "   " << y_right.size() << " \n";
}

float Tree::_impurity_gain(const std::vector<float>& x, const std::vector<uint32_t>& y, float split_threshold) {
    
    //std::cout << " " << x.size() << "   " << y.size() << "\n";
    assert(x.size() == y.size());
    std::vector<uint32_t> y_left;
    std::vector<uint32_t> y_right;
    for(int i=0; i<x.size(); ++i) {
        if(x[i] < split_threshold) {
            y_left.push_back(y[i]);
        } else {
            y_right.push_back(y[i]);
        }
    }
 
    float parent_entropy = utils::compute_entropy(y);
    float left_entropy = utils::compute_entropy(y_left);
    float right_entropy = utils::compute_entropy(y_right);
    
    // std::cout << " ********* y  |  y_left  |  y_right  = " << y.size() << " | " << y_left.size() << " | "
    //      << y_right.size()  << " | " << split_threshold<< " | "
    //      << left_entropy << " | " << right_entropy << " | " << parent_entropy << " | "
    //     << left_entropy + right_entropy - parent_entropy <<"\n";

    return parent_entropy - (y_left.size()*1.0 / y.size() * left_entropy) - (y_right.size()*1.0 / y.size() * right_entropy);
}

BaseNode* Tree::_grow(const std::vector<std::vector<float>>& X, const std::vector<uint32_t>& y, int current_depth) {
    assert(X.size() > 0);
    assert(X.size() == y.size());
    assert(X[0].size() == m_feature_num);
    
    std::map<uint32_t, uint32_t> y_map;
    utils::reduce_y_by_label(y, y_map);

    // 如果只有一个类别或者大于最大深度,返回
    if(y_map.size() == 1 || current_depth >= m_max_depth) {
         // 构造叶子结点
        Leaf* p_leaf = new Leaf();
        p_leaf->set_labels(y);
        p_leaf->set_type("leaf");
        // std::cout << " A a leaf  " << " sample count " << y.size() << "  " << p_leaf->sample_count() << "\n";
        return p_leaf;
    }

    current_depth += 1;
    uint32_t N = X.size();
    uint32_t M = X[0].size();

    // 列采样
    std::vector<uint32_t> select_cols;
    _col_select(m_feature_num, select_cols);

    // 选择最优分裂点
    SplitInfo split_info;
    std::vector<std::vector<float>> x_left, x_right;
    std::vector<uint32_t> y_left, y_right;
    _segment(X, y, select_cols, split_info, x_left, x_right, y_left, y_right);   // @wjg 存在划分不均匀的情况
    // std::cout << " left count & right count =  " << x_left.size() << " " << x_right.size() << "\n";

    //TODO 对叶子结点的最小数目进行限制
    // 如果发现分裂后的left or right 叶子结点为空, 则返回
    if(y_left.size() == 0 || y_right.size() == 0) {
        Leaf* p_leaf = new Leaf();
        p_leaf->set_labels(y);
        p_leaf->set_type("leaf");
        // std::cout << " B a leaf  " << " sample count " << p_leaf->sample_count() << "\n";
        return p_leaf;
    }

    // std::cout << split_info.split_idx << "     "  << split_info.split_threshold << "\n";
    
    BaseNode* _left = _grow(x_left, y_left, current_depth);
    BaseNode* _right = _grow(x_right, y_right, current_depth);

    // 递归的最外层, 构建root节点
    Node* p_node = new Node();
    p_node->set_left(_left);
    p_node->set_right(_right);
    p_node->set_split(split_info.split_idx, split_info.split_threshold);
    p_node->set_type("node");

    // std::cout << "parent: " << p_node << " ,  left : " << p_node->get_left() << " , right : " << p_node->get_right() << "\n\n";

    return p_node;
}

void Tree::predict(const std::vector<float>& X, std::vector<uint32_t>& pred_y) const {
    Leaf* p_leaf = static_cast<Leaf*>(_traverse(m_root, X));
    assert(p_leaf->get_type() == "leaf");
    p_leaf->get_labels(pred_y);
}

void Tree::predict_prob(const std::vector<float>& X, std::map<uint32_t, float>& pred_y_prob) const {
    std::vector<uint32_t> y_hat;
    std::map<uint32_t, uint32_t> y_hat_map;
    
    predict(X, y_hat);
    utils::reduce_y_by_label(y_hat, y_hat_map);

    uint32_t y_count = y_hat.size();
    for(auto it=y_hat_map.begin(); it!=y_hat_map.end(); ++it) {
        pred_y_prob.insert(std::make_pair(it->first, it->second*1. / y_count));
    }
}

void Tree::fit(const std::vector<std::vector<float>>& X, const std::vector<uint32_t>& y) {
    uint32_t samples_num = X.size();
    assert(samples_num > 0);
    assert(X[0].size() == m_feature_num);
    assert(y.size() == samples_num);
    
    
    m_root = _grow(X, y, 0);
    assert(m_root != NULL);
    // std::cout << " tree at:   " << m_root << "\n";
}

} // namespace tree

