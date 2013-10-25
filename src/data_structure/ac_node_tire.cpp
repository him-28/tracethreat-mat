#include "data_structure/ac_node_tire.hpp"

namespace data_structure
{
    NodeStruct::char_t label (const NodeStruct::node_t  p)const
    {
        return node_container[p].label;
    }

    node_ac<NodeStruct>  unit(const NodeStruct::node_t index)
    {
        if(index >= node_container.size()) {
            resiz(index + 1);
        }

        return node_container[index];
    }

    void resize_node(std::size_t new_size)
    {
        if(size <= node_container.size())
            return;

        node_container.resize(new_size);

        std::size_t old_size = node_array_container.size();
        node_array_container.resize(new_size);

        //insert connect size
        for(std::size_t array_count = old_size + 1;
                array_count < node_array_container.size(); ++array_count) {
            node_array_container[array_count - 1].next = array_count;
            node_array_container[array_count].prev     = array_count -1;
        }

        typename NodeStruct::node_t old_tail = node_array_container[head].prev;
        typename NodeStruct::node_t new_tail = new_size - 1;

        node_array_container[old_tail].next = old_size;
        node_array_container[old_size].prev = old_tail;

        node_array_container[head].prev = new_tail;
        node_array_container[new_tail].next = head;
    }

    void  reverse_node(NodeStruct::node_t index)
    {
        node_array_container *n_array = node_array_container[index];
        n_array->used = true;
        node_array_container[n_array->prev].next = n_array->next;
        node_array_container[n_array->next].prev = n_array->prev;
    }

    template<typename NodeStruct>
    void ac_node_tire<NodeStruct>::build_node(std::size_t depth,
            typename NodeStruct::node_t parent,
            typename NodeStruct::node_t begin,
            typename NodeStruct::node_t end)
    {
        if(begin == end)
            return;

        std::vector<NodeStruct::char_t> labels;
        std::vector<NodeStruct::node_t> guards;

        for(NodeStruct::node_t count_str = begin; count_str < end; ++count_str) {
            NodeStruct::char_t label = Label(keyid_container[count_str], depth);

            if(!labels.size() || ( labels.back() != label)) {
                labels.push_back(label);
                guards.push_back(count_str);
            }
        }

        guards.push_back(end);

        NodeStruct::node_t base = fetch_array(labels);
        node_container[parent].base = base;

        insert_node(parent, base, labels);

        if(labels[0] != null_char()) {
            node_container[parent].child_label = labels[0];
        } else {
            node_container[parent].final = true;
            NodeStruct::node_t child = base + index(labels[0]);
            node_container[child].set_value_index(keyid_container[begin]);

            if(labels.size() > 1) {
                node_container[parent].child_label = labels[1];
            }
        }

        for(std::size_t count_num  = node_container[parent].final;
                count_num < labels.size();
                ++count_num) {
            NodeStruct::node_t child = base + index(labels[count_num]);
            build_node(depth + 1, child, guards[count_num], guards[count_num + 1]);
        }

    }

    NodeStruct::node_t fetch_node(const std::vector<NodeStruct::char_t> *labels)
    {
        NodeStruct::node_t  id = node_array_container[head].next;

        while(id != head) {
            NodeStruct::node_t base = id - index(labels[0]);
            bool fetched = true;

            for(std::size_t count_num = 0; count_num < labels.size(); ++count_num) {
                NodeStruct::node_t  p = base + index(labels[count_num]);

                if(p >= num_nodes()) {
                    break;
                } else if(node_array_container[p].used) {
                    fetched = false;
                    break;
                }
            }

            if(fetched) {
                return base;
            }

            id = node_array_container[id].next;
        }

        return num_nodes() - index(lables[0]);
    }

    template<typename NodeStruct>
    void ac_node_tire<NodeStruct>::insert_node(NodeStruct::node_t parent,
            NodeStruct::node_t base,
            const std::vector<NodeStruct::char_t>& labels)
    {
        if(!labels.size())
            return;

        NodeStruct::node_t max_id = base + index(labels.back());
        resize(max_id + 1);
        typename std::vector<NodeStruct::char_t>::iterator iter;
        std::size_t count_id = 0;

        for(iter = labels.begin(); iter != labels.end(); ++iter, ++count_id) {
            NodeStruct::node_t  id_node = base + index(*iter);
            node_ac *ac_n  = unit(id_node);
            reverse_node(id_node);
            ac_n->check = parent;
            ac_n->label = *iter;

            if(count_id + 1 < labels.size()) {
                ac_n->sibling = index(*(iter + 1) - index(*iter));
            } else {
                ac_n->sibling = 0;
            }
        }

    }
}


