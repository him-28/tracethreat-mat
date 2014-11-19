#include "exception/system_exception.hpp"
#include "threadconcurrency/tbbpostscan_pe_task.hpp"

namespace controller
{

    tbbpostscan_pe_task::
    tbbpostscan_pe_task(monitor_controller& monitor, size_t& count, int64_t timeout):
        _monitor(monitor),
        _count(count),
        _timeout(timeout),
				threatinfo(new tbbpostscan_pe_task::threatinfo_type),
        _done(false)
    {


    }

    //    set_file(tbb::concurrent_vector<char> *_binary_hex_input)

    bool tbbpostscan_pe_task::
    set_file(std::vector<char> *_binary_hex_input)
    {

        binary_hex_input_ = _binary_hex_input;
        return true;
    }


    bool tbbpostscan_pe_task::
    set_file_name(const char *_file_name)
    {
        file_name = _file_name;
        return true;
    }
    //Signature support pe type.
    bool tbbpostscan_pe_task::
    set_signature(std::vector<struct utils::meta_sig *> *_msig)
    {
        msig_ = _msig;
        return true;

    };

    bool tbbpostscan_pe_task::
    set_callback(result_callback_type *res_callback)
    {
        call_back_ = res_callback;
        return true;

    }

		tbbpostscan_pe_task::rcb_container_type & 
		tbbpostscan_pe_task::
		get_callback(){
				return call_back_->get_result();
		}

    bool tbbpostscan_pe_task::
    set_sig_engine(actire_sig_engine_type *_actire_engine)
    {
        actire_engine_ = _actire_engine;
        return true;

    }

    bool tbbpostscan_pe_task::
    set_search_engine(iactire_concur_type *_iactire_concur)
    {
        iactire_concur_ =  _iactire_concur;
        return true;

    }

    bool tbbpostscan_pe_task::
    set_point(uint64_t _start_point, uint64_t _end_point)
    {
        start_point = _start_point;
        end_point   = _end_point;
        return true;

    }

		tbbpostscan_pe_task::threatinfo_type * 
		tbbpostscan_pe_task::
		get_threatinfo_ptr(){
				return threatinfo;
		}

		tbbpostscan_pe_task::threatinfo_vec_type & 
		tbbpostscan_pe_task::
		get_threatinfo(){
				return threatinfo_vec; 
		}

    void tbbpostscan_pe_task::
    run()
    {
        goto_ = &actire_engine_->get_goto_fn();
        failure_ = &actire_engine_->get_failure_fn();
        output_ = &actire_engine_->get_output_fn();
	

				threatinfo = new message_tracethreat::InfectedFileInfo(); 
			

        _startTime = util_thread::current_time();

        {

            synchronized s(_sleep);

            try {
                _sleep.wait(_timeout);
            } catch(exceptions::timed_out_exception& e) {

            } catch(...) {
                assert(0);
            }

        }

        _endTime =  util_thread::current_time();

        _done = true;

        {
            synchronized s(_monitor);
  
						//return value to queue system.
        		threatinfo = iactire_concur_->search_parallel(*goto_,
                *failure_,
                *output_,
                *call_back_,
                start_point,
                end_point,
                file_name,
                binary_hex_input_);

						std::cout<<"!Result file Name : " << threatinfo->file_name() <<std::endl;

            _count--;

            if (_count == 0) {
                std::cout<<" Thread notify...: "<<std::endl;
                _monitor.notify();
            }
        }


    }

}
