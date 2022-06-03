#include "utils.h"
#include "dataset.h"
#include "tree.h"

int main(int argc, char** argv) {
    // 首先加载训练数据
    dataset::Dataset dataset("x0", 28);
    std::vector<std::vector<float>> train_x;
    std::vector<std::vector<float>> test_x;
    std::vector<uint32_t> train_y;
    std::vector<uint32_t> test_y;
    if(dataset.load()) {
        dataset.get_train_test_dataset(train_x, train_y, test_x, test_y);
    }
    //dataset.debug();
    //std::cout << train_x.size() << " " << train_y.size() << " " << test_x.size() << " " << test_y.size()  << "\n";
    
    uint32_t feat_num = dataset.get_feature_number();
    uint32_t class_num = dataset.get_class_number();
    uint32_t sample_num = dataset.get_sample_number();
    
    std::cout << "  样本信息 \n  ";
    std::cout << " " << feat_num << " " << class_num << " " <<sample_num << "\n\n";
    

    std::cout << " 开始训练 \n\n";
    std::cout << " 训练样本信息 :  "  << train_x.size() << "\n\n";
    tree::Tree tree(feat_num, class_num, 0.75);
    tree.fit(train_x, train_y);

    std::cout << " Train ok ... \n";
    std::cout << " test accuracy ... " << "\n";
    std::map<uint32_t, float> pred_y;

    int accuracy = 0;
    for(int i=0; i<test_x.size();++i) {
        pred_y.clear();
        tree.predict_prob(test_x[i], pred_y);
        if( test_y[i] == 1 && pred_y[1] >= 0.5 || test_y[i] == 0 && pred_y[0] < 0.5) {
            ++accuracy;
        } 
    }
    std::cout << " accuracy is " << accuracy * 1.0 / test_y.size();

    return 0;
}


