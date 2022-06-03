#ifndef TREE_H_
#define TREE_H_
#include "utils.h"

#define NAGETIVE_INFINIT -1000000 

namespace tree {

// 每个分割节点的分割信息, 分割特征: split_idx 以及 分割的阈值 split_threshold
struct SplitInfo {
    uint32_t split_idx;
    float split_threshold;
};

// BaseNode数据结构, 中间节点和叶子节点都是一种BaseNode
class BaseNode {
public:
    explicit BaseNode() : m_left(NULL)
                        , m_right(NULL)
                        , m_split_index(-1)
                        , m_split_threshold(0.0)
                        , m_type("base")
                         {
                            m_labels.clear();
                         }
    // TODO
    // 关闭赋值、拷贝构造函数

    virtual ~BaseNode() {
        if(m_left) {
            delete m_left;
            m_left = NULL;
        }
        if(m_right) {
            delete m_right;
            m_right = NULL;
        }
    }

    virtual void set_left(BaseNode* p) {m_left = p;}
    virtual void set_right(BaseNode* p) {m_right = p;}
    virtual void set_labels(const std::vector<uint32_t>& y) {};
    virtual void set_split(uint32_t index, float threshold) { m_split_index=index; m_split_threshold = threshold; };
    virtual void set_type(std::string type) {m_type = type;}

    virtual BaseNode* get_left() const {return m_left;};
    virtual BaseNode* get_right() const {return m_right; };
    virtual uint32_t get_split_index() const {return m_split_index;  };
    virtual float get_split_threshold() const { return m_split_threshold; };
    virtual std::string get_type() const { return m_type; };

protected:
    std::vector<uint32_t> m_labels;

private:
    BaseNode* m_left;
    BaseNode* m_right;
    uint32_t m_split_index;
    float m_split_threshold;
    std::string m_type;
};


class Node : public BaseNode {
public:
    explicit Node():BaseNode() {
        set_type("node");
    }
};


class Leaf : public BaseNode {
public:
    explicit Leaf() : BaseNode() {
        set_type("leaf");
    }

    void get_labels(std::vector<uint32_t>& pred_y) {
        assert( get_type() == "leaf"  );
        pred_y.assign(m_labels.begin(), m_labels.end());
    }

    void set_labels(const std::vector<uint32_t>& y) {
        assert( get_type() == "leaf");
        m_labels.assign(y.begin(), y.end());
    }

    uint32_t sample_count() const {
        return m_labels.size();
    }
};


class Tree {
public:
    explicit Tree(uint32_t feat_num=0, uint32_t class_num=2, float col_sample_tr=1.0, int max_depth=6)
                : m_root(NULL)
                , m_feature_num(feat_num)
                , m_class_num(class_num)
                , m_col_sample_ratio(col_sample_tr)
                , m_max_depth(max_depth) {
        m_col_sample_feature_num = static_cast<int>(m_feature_num * m_col_sample_ratio);
        assert(m_col_sample_feature_num > 0 && m_col_sample_feature_num <= m_feature_num);
    }

    void predict(const std::vector<float>& X, std::vector<uint32_t>& pred_y) const;
    void predict_prob(const std::vector<float>& X, std::map<uint32_t, float>& pred_y_prob) const;
    void fit(const std::vector<std::vector<float>>& X, const std::vector<uint32_t>& y);

    void set_feature_number(uint32_t num) {
        m_feature_num = num;
    }
    void set_class_number(uint32_t num) {
        m_class_num = num;
    }

private:
    /**
     * 从根节点开始,沿着分裂路径走到叶子结点, 并返回指向叶子结点的指针
     */
    BaseNode* _traverse(BaseNode* root, const std::vector<float>& X)  const;
    BaseNode* _grow(const std::vector<std::vector<float>>& X, const std::vector<uint32_t>& y, int current_depth=0);
    void _segment(const std::vector<std::vector<float>>& X
        , const std::vector<uint32_t>& y
        , const std::vector<uint32_t>& split_indexs
        , SplitInfo& split_info
        , std::vector<std::vector<float>>& x_left
        , std::vector<std::vector<float>>& x_right
        , std::vector<uint32_t>& y_left
        , std::vector<uint32_t>& y_right);
    void _col_select(uint32_t feature_num, std::vector<uint32_t>& select_columns);
    float _impurity_gain(const std::vector<float>& x, const std::vector<uint32_t>& y, float split_threshold);
    // float _impurity_gain(const std::vector<float>& y, uint32_t split_index, float split_threshold);
private:
    BaseNode* m_root;
    uint32_t m_feature_num;
    uint32_t m_class_num;
    uint32_t m_max_depth;
    float m_col_sample_ratio;
    uint32_t m_col_sample_feature_num;
    std::vector<uint32_t> m_features;
};
} // namespace tree


#endif

