#ifndef _2_OPT_H_
#define _2_OPT_H_

#include <vector>

std::vector<std::pair<int, int>> improve_2_opt(std::vector<std::pair<int, int>> nexts, std::vector<std::vector<int>> matching_cost );
std::vector<std::pair<int, int>> merge_pairs(std::vector<std::pair<int, int>> nexts, std::vector<std::vector<int>> matching_cost );


#endif