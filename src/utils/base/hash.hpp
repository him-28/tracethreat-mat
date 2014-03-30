#ifndef UTIL_HASH_HPP
#define UTIL_HASH_HPP

/*
* Copyright 2014 MTSec, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*  Titles			                                                     Authors	          Date
 * 	-Folly hash concept																				 R.Chatsiri         21/03/2014
 *  -Hash value                                                R.Chatsiri         21/03/2014
 */

#include <cstring>
#include <stdint.h>
#include <string>
#include <utility>
#include <tuple>

namespace util
{
    namespace hash
    {
        // This is the Hash128to64 function from Google's cityhash (available
        // under the MIT License).  We use it to reduce multiple 64 bit hashes
        // into a single hash.
        inline size_t hash_128_to_64(const size_t upper, const size_t lower)
        {
            // Murmur-inspired hashing.
            const size_t kMul = 0x9ddfea08eb382d69ULL;
            size_t a = (lower ^ upper) * kMul;
            a ^= (a >> 47);
            size_t b = (upper ^ a) * kMul;
            b ^= (b >> 47);
            b *= kMul;
            return b;
        }

        template <typename T, typename... Ts>
        size_t hash_combine(const T& t, const Ts& ... ts)
        {
            return hash_combine_generic<StdHasher>(t, ts...);
        }

        // Never used, but gcc demands it.
        template <class Hasher>
        inline size_t hash_combine_generic()
        {
            return 0;
        }

        template <class Hasher, typename T, typename... Ts>
        size_t hash_combine_generic(const T& t, const Ts& ... ts)
        {
            size_t seed = Hasher::hash(t);

            if (sizeof...(ts) == 0) {
                return seed;
            }

            size_t remainder = hash_combine_generic<Hasher>(ts...);
            return hash_128_to_64(seed, remainder);
        }




        class std_hasher
        {

            public:

                template<typename T>
                static size_t hash(const T& t) {

                    return std::hash<T>()(t);
                }

        };



        //////////////////////////////////////////////////////////////////////

        /*
         * Thomas Wang 64 bit mix hash function
         */

        inline uint64_t twang_mix64(uint64_t key)
        {
            key = (~key) + (key << 21);  // key *= (1 << 21) - 1; key -= 1;
            key = key ^ (key >> 24);
            key = key + (key << 3) + (key << 8);  // key *= 1 + (1 << 3) + (1 << 8)
            key = key ^ (key >> 14);
            key = key + (key << 2) + (key << 4);  // key *= 1 + (1 << 2) + (1 << 4)
            key = key ^ (key >> 28);
            key = key + (key << 31);  // key *= 1 + (1 << 31)
            return key;
        }

        /*
         * Inverse of twang_mix64
         *
         * Note that twang_unmix64 is significantly slower than twang_mix64.
         */

        inline uint64_t twang_unmix64(uint64_t key)
        {
            // See the comments in jenkins_rev_unmix32 for an explanation as to how this
            // was generated
            key *= 4611686016279904257U;
            key ^= (key >> 28) ^ (key >> 56);
            key *= 14933078535860113213U;
            key ^= (key >> 14) ^ (key >> 28) ^ (key >> 42) ^ (key >> 56);
            key *= 15244667743933553977U;
            key ^= (key >> 24) ^ (key >> 48);
            key = (key + 1) * 9223367638806167551U;
            return key;
        }

        /*
         * Thomas Wang downscaling hash function
         */

        inline uint32_t twang_32from64(uint64_t key)
        {
            key = (~key) + (key << 18);
            key = key ^ (key >> 31);
            key = key * 21;
            key = key ^ (key >> 11);
            key = key + (key << 6);
            key = key ^ (key >> 22);
            return (uint32_t) key;
        }



    } //hash


    template<class Key>
    struct hasher;

    template<> struct hasher<int64_t> {
        size_t operator()(int64_t key) const {
            return hash::twang_mix64(uint64_t(key));
        }
    };

    template<> struct hasher<uint64_t> {
        size_t operator()(uint64_t key) const {
            return hash::twang_mix64(key);
        }
    };

    // recursion
    template <size_t index, typename... Ts>
    struct TupleHasher {
        size_t operator()(std::tuple<Ts...> const& key) const {
            return hash::hash_combine(
                    TupleHasher<index - 1, Ts...>()(key),
                    std::get<index>(key));
        }
    };

    // base
    template <typename... Ts>
    struct TupleHasher<0, Ts...> {
        size_t operator()(std::tuple<Ts...> const& key) const {
            // we could do std::hash here directly, but hash_combine hides all the
            // ugly templating implicitly
            return hash::hash_combine(std::get<0>(key));
        }
    };



} //util

// Custom hash functions.
namespace std
{
    // Hash function for pairs. Requires default hash functions for both
    // items in the pair.
    template <typename T1, typename T2>
    class hash<std::pair<T1, T2> >
    {
        public:
            size_t operator()(const std::pair<T1, T2>& x) const {
                return hash_combine(x.first, x.second);
            }
    };

    // Hash function for tuples. Requires default hash functions for all types.
    template <typename... Ts>
    struct hash<std::tuple<Ts...>> {
        size_t operator()(std::tuple<Ts...> const& key) const {
            folly::TupleHasher<
            std::tuple_size<std::tuple<Ts...>>::value - 1, // start index
                Ts...> hasher;

            return hasher(key);
        }
    };
} // namespace std






#endif /* UTIL_HASH_HPP */
