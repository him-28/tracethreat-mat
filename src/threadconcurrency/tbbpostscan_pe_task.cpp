#include "exception/system_exception.hpp"
#include "threadconcurrency/tbbpostscan_pe_task.hpp"

namespace controller
{

	
    template<typename MAPPED_FILE, typename SignatureTypeMemory>
    tbbpostscan_pe_task<MAPPED_FILE, SignatureTypeMemory>::
    tbbpostscan_pe_task(monitor_controller& monitor, size_t& count, int64_t timeout):
				_monitor(monitor),
				_count(count),
				_timeout(timeout),
				_done(false)
    {


    }
		
    template<typename MAPPED_FILE, typename SignatureTypeMemory>
    bool tbbpostscan_pe_task<MAPPED_FILE, SignatureTypeMemory>::
    set_file(tbb::concurrent_vector<char> *_binary_hex_input)
    {

        binary_hex_input_ = _binary_hex_input;
        return true;
    }
		/*

    template<typename MAPPED_FILE, typename SignatureTypeMemory>
    bool tbbpostscan_pe_task<MAPPED_FILE, SignatureTypeMemory>::
    set_file_name(const char *_file_name)
    {
        file_name = _file_name;
        return true;
    }
    //Signature support pe type.
    template<typename MAPPED_FILE, typename SignatureTypeMemory>
    bool tbbpostscan_pe_task<MAPPED_FILE, SignatureTypeMemory>::
    set_signature(std::vector<SignatureTypeMemory *> *_msig)
    {
        msig_ = _msig;
        return true;

    };

    template<typename MAPPED_FILE, typename SignatureTypeMemory>
    bool tbbpostscan_pe_task<MAPPED_FILE, SignatureTypeMemory>::
    set_callback(res_callback_type *res_callback)
    {
        call_back_ = res_callback;
        return true;

    }

    template<typename MAPPED_FILE, typename SignatureTypeMemory>
    bool tbbpostscan_pe_task<MAPPED_FILE, SignatureTypeMemory>::
    set_sig_engine(actire_engine_type *_actire_engine)
    {
        actire_engine_ = _actire_engine;
        return true;

    }

    template<typename MAPPED_FILE, typename SignatureTypeMemory>
    bool tbbpostscan_pe_task<MAPPED_FILE, SignatureTypeMemory>::
    set_search_engine(iactire_concur_type *_iactire_concur)
    {
        iactire_concur_ =  _iactire_concur;
        return true;

    }

    template<typename MAPPED_FILE, typename SignatureTypeMemory>
    bool tbbpostscan_pe_task<MAPPED_FILE, SignatureTypeMemory>::
    set_point(uint64_t _start_point, uint64_t _end_point)
    {
        start_point = _start_point;
        end_point   = _end_point;
        return true;

    }

    template<typename MAPPED_FILE, typename SignatureTypeMemory>
    void tbbpostscan_pe_task<MAPPED_FILE, SignatureTypeMemory>::
    run()
    {
        goto_ = &actire_engine_->get_goto_fn();
        failure_ = &actire_engine_->get_failure_fn();
        output_ = &actire_engine_->get_output_fn();

        iactire_concur_->search_basic(goto_,
                failure_,
                output_,
                *call_back_,
                start_point,
                end_point,
                file_name,
                binary_hex_input_);

        _startTime = util_thread::current_time();

        {
            synchronized s(_sleep);

            try {
                _sleep.wait(_timeout);
            } catch(exceptions::timed_out_exception& e) {
                std::cout<<" Thread exceptions::timed_out_exception& " <<std::endl;
            } catch(...) {
                assert(0);
            }
        }

        _endTime =  util_thread::current_time();

        _done = true;

        {
            synchronized s(_monitor);

            std::cout << "Thread-PE" << _count << " completed " << std::endl;

            _count--;

            if (_count == 0) {
                std::cout<<" Thread notify...: "<<std::endl;
                _monitor.notify();
            }
        }


    }
		*/
    template class tbbpostscan_pe_task<MAPPED_FILE_PE, utils::meta_sig>;

}
