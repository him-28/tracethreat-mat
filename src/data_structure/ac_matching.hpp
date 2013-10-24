#ifdef   DATA_STRUCTURE__AC_MATCHING_HPP
#define  DATA_STRUCTURE__AC_MATCHING_HPP

/*                       Titles                                          Authors                        Date
 * -
 */



namespace data_structure
{
    struct NodeStruct {
        typename char_t = char;
        typename value_t = unit32_t;
        typename node_t  = unint32_t;
				
        char_t *begin;
        char_t *end;
        value_t *value;
				std::size_t *offset;
    };

    template<typename NodeStructi::char_t, typename NodeStruct::value_t>
    class ac_matching
    {
        public:
						struct matcher{
							virtual void operator()(const NodeStruct * node_ptr){ }
						};
						
						template<typename NodeStruct::value_t>
						struct matcher_value 
						{
							private:
								NodeStruct::value_t & * values_;
						};
				
        protected:
            typedef NodeStruct node_t;
            virtual void build() = 0;
            virtual void insert(const NodeStruct *node_ptr) = 0;
            virtual matching(const NodeStruct *node_ptr, matcher& match) const = 0;
            bool sort_array;
    };

}
#endif

