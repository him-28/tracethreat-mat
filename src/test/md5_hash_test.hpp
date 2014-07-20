#include "utils/md5_hash.hpp"
#include "utils/hash.hpp"
#include "tbbscan/data_structure/slice.hpp"
#include <vector>


TEST(MD5_HASH, simple_md5_hash)
{
    EXPECT_EQ(utils::convert::md5_hash("The quick brown fox jumps over the lazy dog"),
            0x82b62b379d7d109eLLU);
    EXPECT_EQ(utils::convert::md5_hash("The quick brown fox jumps over the lazy dog."),
            0x1cfbd090c209d9e4LLU);
    EXPECT_EQ(utils::convert::md5_hash(""),
            0x04b2008fd98c1dd4LLU);


}


TEST(HASH, simple_hash_bloom_filter)
{

    std::vector<std::string>  key_;
    std::vector<utils::slice> key_slices;
    key_.push_back("The quick brown fox jumps over the lazy dog");

    for (size_t i = 0; i < key_.size(); i++) {
        key_slices.push_back(utils::slice(key_[i]));
    }
    EXPECT_EQ(utils::hash("The quick brown fox jumps over the lazy dog", key_slices.size(), 0xbc9f1d34), 
													 303176499);

    EXPECT_EQ(utils::hash("0x82b62b379d7d109eLLU", key_slices.size(), 0xbc9f1d34), 
													 585796183);

}
