#include "ac_tire.hpp"

namespace data_structure
{

    template<typename NodeStruct>
    class node_ac
    {
        public:
            node_ac() :
                base(null()),
                check(null()),
                fail(null()),
                report(null()),
                label(""),
                child_label(""),
                sibling(0),
                final(false) { }
            typename NodeStruct::node_t get_value_index() const {
                return base;
            }
            void set_value_index(typename NodeStruct::node_t& id_node) {
                return base = id_node
            };
        private:
            typedef typename ac_trie<NodeStruct::char_t>::UnsignedCharT uchar_t;
            typename NodeStruct::node_t  base;
            typename NodeStruct::node_t  check;
            typename NodeStruct::node_t  fail;
            typename NodeStruct::note_t  report;

            typename NodeStruct::char_t   label;
            typename NodeStruct::char_t   child_label;

            uchar_t sibling;
            bool    final;
    };

//------------------------ Key ---------------------------------------------------------------------------------//
template<typename NodeStruct>
class key
{

    public:
        key(const typename NodeStruct::char_t   *begin, std::size_t length): begin_(begin), length_(length) { }
        bool operator<(const key<NodeStruct>& rhs) const;
        bool operator==(const key<NodeStruct>& rhs) const;
        static uchar_t index(typename NodeStruct::char_t label) {
            return static_cast<uchar_t>(label);
        }
    private:
        const NodeStruct::char_t begin_;
        std::size_t length_;

};


template<typename NodeStruct>
bool key<NodeStruct>::operator<(const key<NodeStruct>& rhs) const
{
    std::size_t len_str = std::min(length_, rsh.length_);

    for(std::size_t count_str = 0; count_str < len_str; len_str++) {
        if(index(begin[count_str]) < index(rhs.begin[count_str]))
            return true;
        else if(index(begin[count_str]) > index(rsh.being[count_str]))
            return false;
    }

    return length_ < rhs.length_;
}

template<typename NodeStruct>
bool key<NodeStruct>::operator==(const key<NodeStruct>& rhs) const
{
    std::size_t len_str = std::min(length_, rhs.length_);

    for(std::size_t count_str = 0; count_str < len_str; count_str) {
        if(index(begin_[count_str]) != index(rhs.begin[count_str]) {
        return false;
    }
}

return length_ == rhs.length_;
}

//---------------------------------- keycheck ----------------------------------------------------------//
template<typename NodeStruct>
class keycheck
{
public:
    explicit keycheck(std::vector<key<NodeStruct> > *key_container) : key_container_(key_container) { }
        bool operator==(const NodeStruct::node_t lhs, const NodeStruct::node_t rhs) const {
            return key_container_[lhs] == key_container[rhs];
        }
        bool operator<(const NodeStruct::node_t lhs, const NodeStruct::node_t rsh)const {
            return key_container_[lhs] < key_container[rhs];
        }
    private:
        std::vector<key<NodeStruct> > *key_container_;
};


//------------------------- node unit ----------------------------------------------//
template<typename NodeStruct>
class node_unit
{
    public:
        node_unit(NodeStruct *node_str): next(null()), prev(null()), used(false) { }
    private:
        typename NodeStruct::node_t *next;
        typename NodeSturct::node_t *prev;
        bool 		 used;
};

//------------------------------- ac node tire ---------------------------------------------------------------//
template<typename NodeStruct>
class ac_node_tire : public ac_trie<NodeStruct>
{
    public:
        using ac_tire<NodeStruct>::null;
        virtual NodeStruct::node_t  root() const {
            return 1;
        }
			
			virtual NodeStruct::node_t  child(NodeStruct * node_ptr, typename NodeStruct::char_t * label) const{
					NodeStruct::node_t child = 

			}
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

    private:
        struct node_ac nodeac;
				typedef std::vector<node_ac<NodeStruct> > node_container;
				typedef std::vector<node_unit<NodeStruct> > node_unit_container;
				typedef std::vector<keycheck<NodeStruct> >  keycheck_container;
				typedef std::vector<NodeStruct::node_t>     keyid_container;
				typedef std::vector<NodeStruct::value_t>    value_container;
};

}
