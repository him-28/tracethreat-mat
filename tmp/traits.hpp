#ifndef DATA_STRUCTURE__TRAITS_HPP
#define DATA_STRUCTURE__TRAITS_HPP

#include <stdint.h>

namespace data_structure
{
	template<int size>
	struct UnsignedIntTrie{  typedef uint32_t UnsignedInt; };

	template<typename NodeStruct>
	struct ac_trie{
		typedef typename UnsignedIntTrie<sizeof(NodeStruct::char_t)>::UnsingedInt UnsignedCharT;
	};

};

#endif
