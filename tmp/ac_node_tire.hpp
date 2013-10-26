#ifndef DATA_STRUCTURE__AC_NODE_TIRE_HPP
#define DATA_STRUCTURE__AC_NODE_TIRE_HPP

#include "ac_tire.hpp"

namespace data_structure
{
    template<typename NodeStruct> class node_ac;
    template<typename NodeStruct> class key;
    template<typename NodeStruct> class keycheck;
    template<typename NodeStruct> class node_array;

		
    //------------------------------- ac node tire ---------------------------------------------------------------//
    template<typename NodeStruct>
    class ac_node_tire : public ac_trie<NodeStruct>
    {
        public:
            typedef typename tire<NodeStruct::char_t>::UnsingedChar UnsignedChar_t;
            using ac_tire<NodeStruct>::null;

            ac_node_tire(): sort_array(true) { }

            virtual NodeStruct::node_t  root() const {
                return 1;
            }

            virtual NodeStruct::node_t& child(NodeStruct *node_ptr,
                    typename NodeStruct::char_t *label) const {
                node_ptr->child = node_container[node_ptr->parent].base + index(label);

                if(node_ptr->child < node_container.size()
                        && node_container[child].check == node_ptr->parent]) {
                    return node_ptr->child;
                }

                return null();

            }

            virtual bool node_final(const NodeStruct::node_t *p) const {

                return node_container[node_container[p].base].check == p;
            }

            virtual NodeStruct::value_t& get_value(NodeStruct::node_t  *p) const {
                if(node_final(p)) {
                    NodeStruct::node_t key_id = node_container[node_container[p].base].get_value_index();
                    return vlaue_container[key_id];
                }

                return null();
            }

            virtual NodeStruct::node_t  first_child(const NodeStruct::node_t parent)  const {
                if(node_container[parent].child_label != null_char()) {
                    return node_container[parent].base + index(node_container[parent].child_label);
                }

                return null();
            }

            virtual NodeStruct::node_t  sibling(const NodeStruct::node_t p) const {
                if(node_container[p].sibling == 0)
                    return null();

                return p + node_container[p].sibling;
            }

            virtual NodeStruct::node_t  fail(const NodeStruct::node_t p) const {
                return node_container[p].fail;

            }

            virtual void set_fail(const NodeStruct *node_ptr) {
                node_container[p]->fail = node_ptr->fail;
            }

            virtual NodeStruct::node_t  report(const NodeStruct *node_ptr) {
                return node_container[p].report;
            }

            virtual void set_report(const NodeStruct *p , NodeStruct::node_t report) {
                node_container[p].report = report;
            }
 
            virtual void insert(const NodeStruct *node_ptr) {
                if(node_ptr->begin != node_ptr->end) {
                    std::size_t length = node_ptr->end - node_ptr->begin;
                    keyid_container.push_back(key<NodeStruct>(node_ptr->begin, length));
                    value_container.push_back(value);
                }
            }

            virtual void build() {
                keyid_container.clear();

                for(std::size_t id  = 0; id < keyid_container.size(); ++id) {
                    keyid_container.push_back(id);

                }

                if(sort_array) {
                    std::sort(keyid_container.being(), keyid_container.end(), new keycheck(keycheck_container));
                    typename keyid_container::iterator new_end =  std::unique(keyid_container.begin(),
                            keyid_container.end(),
                            new keycheck(keycheck_container));
                    keyid_container.resize(new_end - keyid_container.begin());
                    static const NodeStruct::node_t key_head = (1 << (sizeof(UnsignedChar_t) * 8)) + root();
                    head = key_head;
                    node_container.resize(head + 1); // with top node
                    node_array_container.resize(head + 1);
                    node_array_container[head].prev = head;
                    node_array_container[head].next = head;
                    node_ac<NodeStruct> *node_c = unit(root());
                    node_c->check = root();

                    build_node(0, root(), 0 , keyid_container.size());

                    std::vector<keycheck<NodeStruct> >().swap(keycheck_container);
                    std::vector<node_array<NodeStruct> >().swap(node_array_container);
                }
            }

            virtual void clear(){  }
						virtual std::size_t num_nodes()const{
								return node_container.size();
						}
					
						virtual std::size_t node_size()const{
								return sizeof(node_ac);
						}
        private:
            bool sort_array;
            NodeStruct::node_t head;
            struct node_ac nodeac;
            std::vector<node_ac<NodeStruct> >   node_container;
            std::vector<node_array<NodeStruct> > node_array_container;
            std::vector<keycheck<NodeStruct> >  keycheck_container;
            std::vector<NodeStruct::node_t>     keyid_container;
            std::vector<NodeStruct::value_t>    value_container;
    };


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
    class node_array
    {
        public:
            node_array(NodeStruct *node_str): next(null()), prev(null()), used(false) { }
        private:
            typename NodeStruct::node_t *next;
            typename NodeSturct::node_t *prev;
            bool 		 used;
    };


}

#endif
