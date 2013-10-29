
#include "data_structure/ac_tire.hpp"
#include <utility>
#include <iostream>

namespace data_structure
{

    template<typename NodeStruct>
    int ac_tire<NodeStruct>::ac_addstring(struct node<NodeStruct> *node_table,
            const typename NodeStruct::char_t *str,
            int slen)
    {
        struct node_state<NodeStruct> *state, * next;
        state = new node_state<NodeStruct>();
        next  = new node_state<NodeStruct>();
        state->next.resize(AC_CHAR);
        next->next.resize(AC_CHAR);
        std::cout<<"Test zero_state : "<< node_table->zero_state <<std::endl;
        int count_str, done = 0;

        if(node_table->zero_state == 0) {
            node_table->idcounter = 1;
            node_table->zero_state = new node_state<NodeStruct>();
            node_table->zero_state->id = 0;
            node_table->zero_state->depth = 0;
            node_table->zero_state->output;
        }

        count_str = 0;
        state = node_table->zero_state;

        while(!done &&
                (next = &state->next[*(str + count_str)]) != NULL) {
            state = next;

            if( ((slen >= 0)? count_str == slen : *(str + count_str) != '\0')) {
                done = 1;
            }
						std::cout<<" while : count_str : " << count_str <<std::endl;
            count_str++;
        }

        if(!done) {

            while(((slen >= 0) ? count_str < slen : *(str + count_str) != '\0')) {
                next = new node_state<NodeStruct>();

                if(!next) {
                    return 0;
                }

                next->id = node_table->idcounter++;
                next->depth = state->depth + 1;
                next->output = NULL;
                node_state<NodeStruct> *tmp_next = &state->next[*(str + count_str)];
                tmp_next->next[*(str + count_str)] = *next;
                state = next;
                count_str++;
            }
        }

        if(state->output == NULL) {
            state->output = const_cast<char *>(str);
            node_table->pattern_counter++;
        }

        return 0;
    }

    template class ac_tire<NodeStruct>;
}


