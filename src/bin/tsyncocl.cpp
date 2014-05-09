#include "utils/file_offset_handler.hpp"
#include "scan/file_scan_policy.hpp"
#include "data_structure/actire_parallel.hpp"
#include "data_structure/ac_tire.hpp"
#include "CL/cl.h"
#define FILE_ON 1



using namespace data_structure;
using namespace policy;
using namespace utils;

typedef struct {
    std::vector<std::string> keywords;
    char const *input;
} test_data;

int main()
{

}
