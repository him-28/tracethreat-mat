#ifndef DATA_STRUCTURE_VECTOR_BUFFER_HPP
#define DATA_STRUCTURE_VECTOR_BUFFER_HPP

/*                       Titles                                          Authors                        Date
 *- vector devices                                                       Chatsiri.rat								  14/01/2013
 *- add exception & throw exception for handeling member functions       Chatsiri.rat                 17/01/2013
 */

//BOOST
#include <boost/shared_ptr.hpp>

//Internal
#include "exception/datastructure_exception.hpp"
#include "utils/logger/clutil_logger.hpp"

namespace data_structure
{

    using namespace boost;
    namespace exp_container = trace_exception::container;

    template<typename BufferType>
    class vector_buffer
    {
        public:
            vector_buffer();
            void initial_size(unsigned int size);
            void push_back(BufferType const&   data);
            BufferType *pop_index(unsigned int index);
            BufferType& operator [](unsigned int  index_buffer)throw(exp_container::container_exception);
            unsigned int size()const;
        private:
            int size_;
            BufferType *buffer_data_;
            int counter_size;

            // logging
            shared_ptr<utils::clutil_logging<std::string, int> > *logger_ptr;
            utils::clutil_logging<std::string, int>   *logger;

    };

    /**
    * @brief Size and Counter Size defaults size is zero.
    */
    template<typename BufferType>
    vector_buffer<BufferType>::vector_buffer() : size_(0), counter_size(0), buffer_data_(NULL)
    {
        //init logger
        logger_ptr = &utils::clutil_logging<std::string, int>::get_instance();
        logger = logger_ptr->get();
        logger->write_info("Load vector buffer");
    }

    /**
    * @brief Pop data back by index of array.
    *
    * @param index  index of array.
    *
    * @return return buffer_data_[index], It's index of array.
    */
    template<typename BufferType>
    BufferType *vector_buffer<BufferType>::pop_index(unsigned int index)
    {
        try {
            if(size_ == 0 || index > counter_size || index > size_)
                throw exp_container::container_exception("[** Size of vector is zero or index more than counter size value **]");
            else
                return &buffer_data_[index];
        } catch(exp_container::container_exception& cont_exception) {
            logger->write_info("Error, pop_index ", cont_exception.message_error_size_initial());
            delete buffer_data_;
            return NULL;
        }
    }

    /**
    * @brief Size of array declare for creating vector
    *
    * @param size  set size of array
    */
    template<typename BufferType>
    void vector_buffer<BufferType>::initial_size(unsigned int size)
    {
        size_ = size;
        buffer_data_ = new BufferType[size_];
    }

    /**
    * @brief push_back,Push data to tail of array.
    *
    * @param data  Input data, type define generics.
    *
    */
    template<typename BufferType>
    void vector_buffer<BufferType>::push_back(BufferType const&   data)
    {
        try {
            if(size_ ==  0) {
                throw exp_container::container_exception(" [** Size not initial **] ");
            } else {
                (buffer_data_)[counter_size] = data;
                logger->write_info("--- Insert data to array ", lexical_cast<std::string>(&buffer_data_[counter_size]));
                counter_size++;

            }
        } catch(exp_container::container_exception& cont_exception) {
            delete buffer_data_;
            logger->write_info("Error, push_back ", cont_exception.message_error_size_initial());
        }

    }

    /**
    * @brief Operator [] of array
    *
    * @param index_buffer  input index of array, index must have not value more than size of array
    *
    * @return return index address of array.
    *
    * @throw exp_container::container_exception
    */
    template<typename BufferType>
    BufferType& vector_buffer<BufferType>::
			operator [](unsigned int index_buffer) throw(exp_container::container_exception)
    {
        if(index_buffer > size_)
            throw exp_container::container_exception("Cannot get more size of index");

        //logger->write_info("-- Operator[ ] insert index ", boost::lexical_cast<std::string>(index_buffer));
        return buffer_data_[index_buffer];
    }

    template<typename BufferType>
    unsigned int vector_buffer<BufferType>::size()const
    {
        return size_;
    }
}

#endif /* HNMAV_DATASTRUCTURE_VECTOR_BUFFER_HPP */
