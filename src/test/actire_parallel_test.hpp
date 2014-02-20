#include "data_structure/actire_parallel.hpp"

/**
* @brief Create Testing insert data to opencl string.
*/
using namespace data_structure;

class ACTireParallelTest : public ::testing::Test
{
	protected:
			virtual void SetUp(){


		  }
		 boost::unordered_map<char, size_t>  edges_t;
		 std::vector<edges_t> graph_;
		 
};
