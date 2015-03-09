#ifndef CONTROLLER_TBBPOSTSCAN_PE_TASK_HPP
#define CONTROLLER_TBBPOSTSCAN_PE_TASK_HPP

#include <vector>

#include "tbbscan/data_structure/actire_concurrency.hpp"

#include "threadconcurrency/thread_manager.hpp"
#include "threadconcurrency/monitor_controller.hpp"
#include "threadconcurrency/platformthread_factory.hpp"
#include "threadconcurrency/util_thread.hpp"

#include "msg/message_tracethreat.pb.h"


namespace controller
{

    namespace exceptions = hnmav_exception::controller;
	
    class tbbpostscan_pe_task : public Runnable
    {
        public:
						
            typedef tbbscan::actire_sig_engine<char, true>  actire_sig_engine_type;

            typedef tbbscan::iactire_engine<char, true> iactire_concur_type;

            typedef tbbscan::goto_function<char, true>  goto_type;

            typedef tbbscan::failure_function<char , true>  failure_type;

            typedef tbb::concurrent_unordered_map<std::size_t, std::set<struct utils::meta_sig *> >
                    output_type;

						typedef scan_threat::InfectedFileInfo  threatinfo_type;

					  typedef std::vector<threatinfo_type*>  threatinfo_vec_type;

						typedef std::vector<threatinfo_type*>  rcb_container_type;
	
            typedef struct tbbscan::result_callback<rcb_container_type, threatinfo_type> 
							result_callback_type;

            tbbpostscan_pe_task(monitor_controller& monitor, size_t& count, int64_t timeout);
	
            bool set_file(std::vector<char> *_binary_hex_input);
				
						bool set_file_name(const char * _file_name);
 
           //Signature support pe type.
            bool set_signature(std::vector<struct utils::meta_sig*> *_msig);

            bool set_callback(result_callback_type *res_callback);

						rcb_container_type & get_callback();				

            bool set_sig_engine(actire_sig_engine_type *_actire_engine);

            bool set_search_engine(iactire_concur_type *_iactire_concur);

						bool set_point(uint64_t _start_point, uint64_t _end_point);

						threatinfo_type * get_threatinfo_ptr();

						threatinfo_vec_type & get_threatinfo();

            void run();
					 
        private:

            std::vector<struct MAPPED_FILE_PE_PE *>   *mapped_file_vec_;
						
            actire_sig_engine_type *actire_engine_;

            iactire_concur_type   *iactire_concur_;

            std::vector<struct utils::meta_sig*>   *msig_;

            goto_type 					*goto_;
            failure_type 				*failure_;
            output_type   			*output_;
            result_callback_type   *call_back_;
            uint64_t       			start_point;
            uint64_t     	 			end_point;
            const char 					*file_name;
            //tbb::concurrent_vector<char> *binary_hex_input_;
				    std::vector<char> *binary_hex_input_;



            size_t& _count;
            int64_t _timeout;
            int64_t _startTime;
            int64_t _endTime;
            bool _done;

            monitor_controller& _monitor;

						monitor_controller _sleep;

						threatinfo_type *    threatinfo;			
	
						threatinfo_vec_type  threatinfo_vec;	
    };

}




#endif /* CONTROLLER_TBBPOSTSCAN_PE_TASK_HPP */
