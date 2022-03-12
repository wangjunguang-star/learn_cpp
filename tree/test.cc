// void test(Node* p_node) {}

// // void test(BaseNode* b_base) {}


// int mian() {
//     std::map<std::string, float> _map;
//     _map.insert(std::make_pair("aaa", 1.0));
//     _map.insert(std::make_pair("bbbb", 2.0));
//     for(auto it=_map.begin(); it!=_map.end(); ++it) {
//         std::cout << it->first << "\t" << it->second << "\n";
//     }
//     std::pair<std::map<std::string,float>::iterator, bool> ret;
//     ret = _map.insert(std::make_pair("aaa", 9.0));
//     if(ret.second == false) {
//         std::cout << " aaa already existed : " << ret.first->second << "\n"; 
//     }
//     ret = _map.insert(std::make_pair("cccc", 1000.0));
//     if(ret.second == false) {
        
//     }

//     // node也是一个basenode，所以一个basenode的指针或者引用，可以自动指向一个派生对象或者引用
//     BaseNode* p_base;
//     Node node;
//     p_base = &node;
//     BaseNode& r_base = node;

//     foo(p_base);
//     foo(*p_base);
//     foo(node);

//     // 反之不行
//     Node* p_node;
//     BaseNode base_node;
//     p_node = &base_node;   // error

//     test(p_node);
//     test(static_cast<Node*>(p_base));


//     return 0;
// }






