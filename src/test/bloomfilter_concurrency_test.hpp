#include "tbbscan/data_structure/bloomfliter_concurrency.hpp"
#include "tbbscan/data_structure/slice.hpp"
#include "utils/hash.hpp"


using utils::slice;

static const int kVerbose = 1;

static utils::slice Key(int i, char *buffer)
{
    memcpy(buffer, &i, sizeof(i));
    return utils::slice(buffer, sizeof(i));
}

class BloomTest
{
    private:
        const utils::filter_policy *policy_;
        std::string filter_;
        std::vector<std::string> keys_;

    public:
				BloomTest() : policy_(new concurrency::bloom_filter_policy(10)) { }

        ~BloomTest() {
            delete policy_;
        }

        void Reset() {
            keys_.clear();
            filter_.clear();
        }

        void Add(const utils::slice& s) {
            keys_.push_back(s.ToString());
        }

        void Build() {
            std::vector<utils::slice> key_slices;

            for (size_t i = 0; i < keys_.size(); i++) {
                key_slices.push_back(slice(keys_[i]));
            }

            filter_.clear();
            policy_->create_filter(&key_slices[0], key_slices.size(), &filter_);
            keys_.clear();

            if (kVerbose >= 2) DumpFilter();
        }

        size_t FilterSize() const {
            return filter_.size();
        }

        void DumpFilter() {
            fprintf(stderr, "F(");

            for (size_t i = 0; i+1 < filter_.size(); i++) {
                const unsigned int c = static_cast<unsigned int>(filter_[i]);

                for (int j = 0; j < 8; j++) {
                    fprintf(stderr, "%c", (c & (1 <<j)) ? '1' : '.');
                }
            }

            fprintf(stderr, ")\n");
        }

        bool Matches(const utils::slice& s) {
            if (!keys_.empty()) {
                Build();
            }

            return policy_->key_may_match(s, filter_);
        }

        double FalsePositiveRate() {
            char buffer[sizeof(int)];
            int result = 0;

            for (int i = 0; i < 10000; i++) {
                if (Matches(Key(i + 1000000000, buffer))) {
                    result++;
                }
            }

            return result / 10000.0;
        }
};

TEST(BloomTest, EmptyFilter)
{
		BloomTest bloomtest;
    ASSERT_TRUE(! bloomtest.Matches("hello"));
    ASSERT_TRUE(! bloomtest.Matches("world"));
}

TEST(BloomTest, Small)
{
		BloomTest bloomtest;

    bloomtest.Add("hello");
    bloomtest.Add("world");
    ASSERT_TRUE(bloomtest.Matches("world"));
    ASSERT_TRUE(bloomtest.Matches("hello"));
    ASSERT_TRUE(! bloomtest.Matches("x"));
    ASSERT_TRUE(! bloomtest.Matches("foo"));
}

