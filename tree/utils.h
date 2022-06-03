#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>
#include <assert.h>
#include <set>

namespace utils {

bool CHECK_EQ(int a1, int a2);
void split_string(std::string& line, char delimiter, std::vector<std::string>* r);
void split_string(std::string& line, char delimiter, std::vector<std::string>& r);
int split_string_n(std::string& line, char delimiter, std::vector<std::string>& r);
void reduce_y_by_label(const std::vector<uint32_t>& y, std::map<uint32_t, uint32_t>& y_map);
float compute_entropy(const std::vector<uint32_t>& y);

} // namespace utils
