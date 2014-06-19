#ifndef CONTROLLER_TBBPOSTSCAN_PE_TASK_HPP
#define CONTROLLER_TBBPOSTSCAN_PE_TASK_HPP

#include "tbbscan/data_structure/actire_concurrency.hpp"

#include "threadconcurrency/thread_manager.hpp"
#include "threadconcurrency/monitor_controller.hpp"
#include "threadconcurrency/platformthread_factory.hpp"
#include "threadconcurrency/util_thread.hpp"

namespace controller
{

    namespace exceptions = hnmav_exception::controller;
	
    template<typename MAPPED_FILE, typename SignatureTypeMemory>
    class tbbpostscan_pe_task// : public Runnable
    {
        public:
						/*
            typedef struct tbbscan::results_callback<std::vector<std::string> > res_callback_type;

            typedef tbbscan::actire_engine_concurrency<char, true>  actire_engine_type;

            typedef tbbscan::iactire_concurrency<char, true> iactire_concur_type;

            typedef tbbscan::goto_function<char, true>  goto_type;

            typedef tbbscan::failure_function<char , true>  failure_type;

            typedef tbb::concurrent_unordered_map<std::size_t, std::set<struct utils::meta_sig *> >
                    output_type;
						
            tbbpostscan_pe_task(monitor_controller& monitor, size_t& count, int64_t timeout);
            //Binary hex.
            bool set_file(tbb::concurrent_vector<char> *_binary_hex_input);
					
						bool set_file_name(const char * _file_name);
            //Signature support pe type.
            bool set_signature(std::vector<SignatureTypeMemory*> *_msig);

            bool set_callback(res_callback_type *res_callback);

            bool set_sig_engine(actire_engine_type *_actire_engine);

            bool set_search_engine(iactire_concur_type *_iactire_concur);

						bool set_point(uint64_t _start_point, uint64_t _end_point);

            void run();
					 */
        private:

            std::vector<struct MAPPED_FILE_PE *>   *mapped_file_vec_;
						/*
            actire_engine_type *actire_engine_;

            iactire_concur_type   *iactire_concur_;

            std::vector<SignatureTypeMemory*>   *msig_;

            goto_type 					*goto_;
            failure_type 				*failure_;
            output_type   			*output_;
            res_callback_type   *call_back_;
            uint64_t       			start_point;
            uint64_t     	 			end_point;
            const char 					*file_name;
            tbb::concurrent_vector<char> *binary_hex_input_;


            size_t& _count;
            int64_t _timeout;
            int64_t _startTime;
            int64_t _endTime;
            bool _done;

            monitor_controller& _monitor;

						monitor_controller _sleep;
					*/
    };

}




#endif /* CONTROLLER_TBBPOSTSCAN_PE_TASK_HPP */
