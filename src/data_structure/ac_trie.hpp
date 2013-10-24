#ifndef DATA_STRUCTURE__AC_TRIE_HPP
#define DATA_STRUCTURE__AC_TRIE_HPP

template<typename NodeStruct>
class ac_trie
{
	public:
			typedef NodeStruct::node_t  node_type;
			virtual ~ac_trie(){  }
			struct node_tire null()
			{
				return 0;
			}

			static bool is_null(NodeStruct::node_t p)
			{
					return  p == null();
			}
			
			virtual NodeStruct::node_t  root() const = 0;
			virtual NodeStruct::node_t  child(NodeStruct * node_ptr, typename NodeStruct::char_t * label) const = 0;
			virtual bool								node_final(NodeStruct * p) const = 0;
			virtual NodeStruct::value_t get_value(NodeStruct *  p) const = 0;
			
			virtual NodeStruct::node_t  first_child(const NodeStruct::node_t parent)  const = 0;
			virtual NodeStruct::node_t  sibling(const NodeStruct::node_t p) const = 0;
			virtual NodeStruct::node_t  fail(const NodeStruct::node_t p) const = 0;
			virtual void                set_fail(const NodeStruct * node_ptr) = 0;
			virtual NodeStruct::node_t  report(const NodeStruct * node_ptr) = 0;
			virtual void                set_report(const NodeStruct * p , NodeStruct::node_t report) = 0;
			
			virtual void insert(const NodeStruct * node_ptr) = 0;
			virtual void build() = 0;
			virtual void clear() = 0;	 
};

#endif


