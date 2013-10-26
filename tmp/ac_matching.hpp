#ifdef   DATA_STRUCTURE__AC_MATCHING_HPP
#define  DATA_STRUCTURE__AC_MATCHING_HPP

/*                       Titles                                          Authors                        Date
 * -
 */



namespace data_structure
{
    struct NodeStruct;

    //-------------------------------- Inheritance ac_node_tire ------------------------
    template<typename NodeStruct::char_t, typename NodeStruct::value_t>
    class ac_matching
    {
        public:
            ac_mathing() : flag_not_built_(true) { }
            // insert overload operator
            bool insert(const NodeStruct::char_t *begin,
                    const NodeStruct::char_t *end,
                    const NodeStruct::value_t& value) {
                if(flag_not_built_) {
                    insert(begin, end, value);
                }

                return flag_not_built_;
            }
            //
            bool insert(const NodeStruct::char_t *begin,
                    std::size_t length,
                    const NodeStruct::value_t& value) {
                if(flag_not_built_) {
                    insert(begin, begin + length, value);
                }

                return flag_not_built_;
            }
            //
            bool insert(const NodeStruct::char_t *begin,
                    const NodeStruct::value_t *value) {
                if(flag_not_built_) {
                    std::size_t length = std::char_traits<NodeStruct::value_t>::length(begin);
                    insert(begin, begin + length, value);
                }

                return flag_not_built_;
            }

            bool build_all(bool sort_array =  true) {
                if(flag_not_built_) {
                    flag_not_built_ = false;
                    build();
                    return true;
                }

                return false;
            }

            template<typename NodeStruct::value_t>
            std::size_t match(const NodeStruct::char_t *begin,
                    const NodeStruct::char_t *end,
                    NodeStruct::value_t *values,
                    bool clear = true) const {
                if (values && clear) values->clear();

                if (values) {
                    matcher_value<NodeStruct::value_t> m(values);
                    return match(begin, end, m);
                } else {
                    static matcher s_m;
                    return match(begin, end, s_m);
                }
            }

            template<typename NodeStruct::value_t>
            std::size_t match(const NodeStruct::char_t *begin,
                    std::size_t length,
                    NodeStruct::value_t *values,
                    bool clear = true) const {
                return match(begin, begin + length, values, clear);
            }

            template<typename NodeStruct::value_t>
            std::size_t match(const NodeStruct::char_t *begin,
                    NodeStruct::value_t *values,
                    bool clear = true) const {
                std::size_t length = std::char_traits<NodeStruct::char_t>::length(begin);
                return match(begin, begin + length, values, clear);
            }

            std::size_t match(const NodeStruct::char_t *begin,
                    const NodeStruct::char_t *end) const {
                static matcher s_m;
                return matche(begin, end, s_m);
            }

            std::size_t match(const NodeStruct::char_t *begin,
                    std::size_t length) const {
                return match(begin, begin + length);
            }

            std::size_t match(const NodeStruct::char_t *begin) const {
                std::size_t length = std::char_traits<NodeStruct::char_t>::length(begin);
                return match(begin, begin + length);
            }

            void clear() {
                flag_not_built_ = true;
            }

            virtual std::size_t node_size() const = 0;
            virtual std::size_t num_nodes() const = 0;



        protected:
            typedef NodeStruct node_t;
            virtual void build() = 0;
            virtual void insert(const NodeStruct *node_ptr) = 0;
            virtual match(const NodeStruct *node_ptr, matcher& match) const = 0;
            bool sort_array;
            bool flag_not_built_;
    };

    //--------------------------------- matcher -------------------------------------
    template<typename NodeStruct>
    class matcher
    {
        public:
            virtual void operator()(const NodeStruct *node_ptr) { }
    };

    template<typename value = NodeStruct::value_t>
    struct matcher_value : public matcher {
        public:
            matcher_value(value *values ) :values_(value) { }

            virtual void operator()(const value& values) {
                values_->push_back(values)
            }

        private:
            NodeStruct::value_t *values_;
    };

    //----------------------------- NodeStruct ----------------------------
    struct NodeStruct {
        typename char_t = char;
        typename value_t = unit32_t;
        typename node_t  = unint32_t;

        char_t *begin;
        char_t *end;
        value_t *value;
        std::size_t *offset;

        node_t *prev;
        node_t *child;
        node_t *fail;
    };

}
#endif

