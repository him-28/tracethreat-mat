#include "filetypes/pe_file_controller.hpp"
#include "boost/lexical_cast.hpp"
#include "threadconcurrency/cliprescan_pe_controller.hpp"

using namespace controller;

namespace filetypes
{

    template<typename MAPPED_FILE>
    pe_file_controller<MAPPED_FILE>::pe_file_controller()
    {

    }

    //utils::scan_file_code
    /**
    * @brief PE_file_controller::scan() for scan binary or MD5,SHA-256 and SSDeep matchs Signature file.
    *
    * @param mapped_file_pe Map file detail such binary and md5 of file.
    * @param sig_shm Shared Memory(SHM)-Signature in memory.
    * @param sig_engine Signature Engine contain in AC-Tire.
    * @param iactire_engine_scanner  AC-Tire Engine for scanning.
    *
    * @return Message of Infected File detail in msg/ directory
    */
    template<typename MAPPED_FILE>
    typename pe_file_controller<MAPPED_FILE>::threatinfo_vec_type &
    pe_file_controller<MAPPED_FILE>::scan(std::vector<MAPPED_FILE *> *mapped_file_pe,
            signature_shm_type  *sig_shm,
            signature_engine_type *sig_engine,
            iactire_engine_scanner_type   *iactire_engine_scanner)
    {
        //PE_FILE_CONTROLLER call AC-DFS algorithms.
        logger->write_info("Start pe_file_controller<MAPPED_FILE>::scan actire-parallel tbb",
                hnmav_util::format_type::type_header);

        typename std::vector<MAPPED_FILE *>::iterator iter_mapped_files;

        size_t summary_file_size = 0;

        //summary file size of all
        for(iter_mapped_files = mapped_file_pe->begin();
                iter_mapped_files != mapped_file_pe->end();
                ++iter_mapped_files) {
            MAPPED_FILE   *mf_pe = *iter_mapped_files;
            summary_file_size += mf_pe->size;
        }//end-for loop

        f_shm_handler.set_shm_name(uuid_gen.generate());
        f_shm_handler.initial_shm(summary_file_size);
        f_shm_handler.initial_file_shm(mapped_file_pe);

        //[x] initial step for scanning multiple file with actire-parallel.
        //[x]Add Signature Engine to thread controller.
        tbbpostscan_pe_col.add_sig_engine(sig_engine);

        //[x]Add Search PE Engine to thread controller.
        tbbpostscan_pe_col.add_search_engine(iactire_engine_scanner);

        tbbpostscan_pe_col.init_syntbb_workload(f_shm_handler.get_map_str_shm(),
                sig_shm,
                f_shm_handler.get_map_file_size(),
                mapped_file_pe);

        tbbpostscan_pe_col.task_start();


        f_shm_handler.delete_file_shm();

        return tbbpostscan_pe_col.get_threatinfo();

    }

    /**
    * @brief Pe_file_controller::scan() overload parameter support OCL scans virus on GPU based.
    *
    * @param symbol_vec  Insert Symbol such as signature file.
    * @param state_vec   State of Symbol.
    * @param mapped_file_pe_vec  Vector contains file details.
    * @param kernel_file_path_ptr  Kernel path such extension .ocl
    * @param sig_shm Shared Memory (SHM) of Signature.
    *
    * @return Success code.
    */
    template<typename MAPPED_FILE>
    utils::scan_file_code  pe_file_controller<MAPPED_FILE>::scan(std::vector<char> *symbol_vec,
            std::vector<size_t> *state_vec,
            std::vector<MAPPED_FILE *> *mapped_file_pe_vec,
            std::string *kernel_file_path_ptr,
            memory::signature_shm<struct memory::meta_sig, struct memory::meta_sig_mem> *sig_shm)
    {

        typename std::vector<MAPPED_FILE *>::iterator iter_mapped_files;
        uint64_t summary_file_size = 0;

        //summary file size of all
        for(iter_mapped_files = mapped_file_pe_vec->begin();
                iter_mapped_files != mapped_file_pe_vec->end();
                ++iter_mapped_files) {
            MAPPED_FILE   *mf_pe = *iter_mapped_files;
            summary_file_size += mf_pe->size;
        }//end-for loop


        //logger->write_info("pe_file_policy::scan_file_type(), Initial file-shm size completed.");


        //pre-scan
        /* - Comment for test with OCL flow only.
        int64_t timeout_scan = 1000LL;
        controller::cliprescan_pe_controller<MAPPED_FILE>  prescan_pe;
        prescan_pe.initial_task_size(mapped_file_pe_vec->size(), timeout_scan, mapped_file_pe_vec);
        prescan_pe.task_start();
        */

        //post-scan
        f_shm_handler.initial_shm(summary_file_size);
        f_shm_handler.initial_file_shm(mapped_file_pe_vec);

        //logger->write_info("pe_file_policy::scan_file_type(), Intial  file-shm data completed.");

        //Send thread pointer to pe_file_controler::scan. Binary file add to tsync before call start()
        //member function of comm_thread_buff.
        //Send vector contains symbol and state.
        //send data to OCL
        tsync.init_syncocl_workload(f_shm_handler.get_map_str_shm(),
                f_shm_handler.get_map_file_size(),
                sig_shm);

        //logger->write_info("pe_file_policy::scan_file_type(), Initial OCL workload completed.");

        tsync.add_load_ocl_system(&load_ocl_system,
                kernel_file_path_ptr,
                symbol_vec,
                state_vec);

        //logger->write_info("pe_file_policy::scan_file_type(), OCL workload completed.");

        //tsync.add_sig_process(symbol_vec, state_vec);

        //logger->write_info("pe_file_policy::scan_file_type(), Add signature completed.");

        tsync.start_processes();

        //logger->write_info("pe_file_policy::scan_file_type(), Processes completed.");

        //tsync.get_slot_ocl_result();

        //TODO : Create struct contains result from OCL.: Stuct contains result with pointer.
        //return std::vector<char>();
    }// end scan


    template<typename MAPPED_FILE>
    bool pe_file_controller<MAPPED_FILE>::set_opencl_file(std::string& kernel_file_path_ptr)
    {
        if(kernel_file_path_ptr.size() == 0) return false;

        this->kernel_file_path_ptr = &kernel_file_path_ptr;
    }

    template<typename MAPPED_FILE>
    inline int16_t pe_file_controller<MAPPED_FILE>::convert_ec16(uint16_t *buff)
    {
        int16_t ret;
        ret = buff[0] & 0xff;
        ret |= (buff[1] & 0xff) << 8;
        return ret;
    }

    template<typename MAPPED_FILE>
    inline int32_t pe_file_controller<MAPPED_FILE>::convert_ec32(uint16_t *buff)
    {
        int32_t ret;
        ret = buff[0] & 0xff;
        ret |= (buff[1] & 0xff) << 8;
        ret |= (buff[2] & 0xff) << 16;
        ret |= (buff[3] & 0xff) << 24;
        return ret;
    }



    template<typename MAPPED_FILE>
    std::list<std::string> pe_file_controller<MAPPED_FILE>
    ::list_pe_header(struct IMAGE_NT_HEADERS *image_nt_header)
    {
        struct IMAGE_FILE_HEADER *pe_file_header = &image_nt_header->FileHeader;

        // Detail from pe.c
        switch(EC16(pe_file_header->Machine)) {
        case 0x0:
            logger->write_info("Machine type: Unknown\n");
            break;

        case 0x14c:
            logger->write_info("Machine type: 80386\n");
            break;

        case 0x14d:
            logger->write_info("Machine type: 80486\n");
            break;

        case 0x14e:
            logger->write_info("Machine type: 80586\n");
            break;

        case 0x160:
            logger->write_info("Machine type: R30000 (big-endian)\n");
            break;

        case 0x162:
            logger->write_info("Machine type: R3000\n");
            break;

        case 0x166:
            logger->write_info("Machine type: R4000\n");
            break;

        case 0x168:
            logger->write_info("Machine type: R10000\n");
            break;

        case 0x184:
            logger->write_info("Machine type: DEC Alpha AXP\n");
            break;

        case 0x284:
            logger->write_info("Machine type: DEC Alpha AXP 64bit\n");
            break;

        case 0x1f0:
            logger->write_info("Machine type: PowerPC\n");
            break;

        case 0x200:
            logger->write_info("Machine type: IA64\n");
            break;

        case 0x268:
            logger->write_info("Machine type: M68k\n");
            break;

        case 0x266:
            logger->write_info("Machine type: MIPS16\n");
            break;

        case 0x366:
            logger->write_info("Machine type: MIPS+FPU\n");
            break;

        case 0x466:
            logger->write_info("Machine type: MIPS16+FPU\n");
            break;

        case 0x1a2:
            logger->write_info("Machine type: Hitachi SH3\n");
            break;

        case 0x1a3:
            logger->write_info("Machine type: Hitachi SH3-DSP\n");
            break;

        case 0x1a4:
            logger->write_info("Machine type: Hitachi SH3-E\n");
            break;

        case 0x1a6:
            logger->write_info("Machine type: Hitachi SH4\n");
            break;

        case 0x1a8:
            logger->write_info("Machine type: Hitachi SH5\n");
            break;

        case 0x1c0:
            logger->write_info("Machine type: ARM\n");
            break;

        case 0x1c2:
            logger->write_info("Machine type: THUMB\n");
            break;

        case 0x1d3:
            logger->write_info("Machine type: AM33\n");
            break;

        case 0x520:
            logger->write_info("Machine type: Infineon TriCore\n");
            break;

        case 0xcef:
            logger->write_info("Machine type: CEF\n");
            break;

        case 0xebc:
            logger->write_info("Machine type: EFI Byte Code\n");
            break;

        case 0x9041:
            logger->write_info("Machine type: M32R\n");
            break;

        case 0xc0ee:
            logger->write_info("Machine type: CEE\n");
            break;

        case 0x8664:
            logger->write_info("Machine type: AMD64\n");
            break;

        default:
            logger->write_info("Machine type: ** UNKNOWN ** (0x%x)\n",
                    boost::lexical_cast<std::string>(EC16(pe_file_header->Machine)));
        }
    }

    template class pe_file_controller<struct MAPPED_FILE_PE>;
}
