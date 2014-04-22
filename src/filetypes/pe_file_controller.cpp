#include "filetypes/pe_file_controller.hpp"
#include "boost/lexical_cast.hpp"
//#include "utils/logger/format_logger.hpp"
//#include "utils/logger/clutil_logger.hpp"

namespace filetypes
{

    template<typename MAPPED_FILE>
    pe_file_controller<MAPPED_FILE>::pe_file_controller()
    {
        //logger
        logger_ptr = &h_util::clutil_logging<std::string, int>::get_instance();
        logger = logger_ptr->get();
        logger->write_info_test("Init logger pe_file_controller");
    }

    template<typename MAPPED_FILE>
    std::vector<struct IMAGE_NT_HEADERS *>&
    pe_file_controller<MAPPED_FILE>::get_pe_header(std::vector<MAPPED_FILE *> *mapped_file_vec)
    {

        logger->write_info("Intial PE header, pe_file_controller<MAPPED_FILE>::get_pe_header...");
        logger->write_info_test("pe_file_controller<MAPPED_FILE>::get_pe_header, vector_size : ",
                boost::lexical_cast<std::string>(mapped_file_vec->size()));

        // PE Header conntained vector. Controller pointer by shared_ptr
        boost::shared_ptr<std::vector<struct IMAGE_NT_HEADERS *> > mapped_vec_shared
            = boost::make_shared<std::vector<struct IMAGE_NT_HEADERS * > >();
        pe_header_vec_shared.push_back(mapped_vec_shared);

        PIMAGE_DOS_HEADER dos_header;
        PIMAGE_NT_HEADERS nt_header;
        size_t headers_size = 0;
        typename std::vector<MAPPED_FILE *>::iterator  iter_mf_vec;
        MAPPED_FILE *mapped_file_ptr;

        for(iter_mf_vec = mapped_file_vec->begin();
                iter_mf_vec != mapped_file_vec->end();
                ++iter_mf_vec) {
            mapped_file_ptr = *iter_mf_vec;

            if(*mapped_file_ptr->data < sizeof(struct IMAGE_DOS_HEADER)) {
                logger->write_info("Mappper data < IMAGE_DOS_HEADER");
                continue;
            }

            dos_header = (PIMAGE_DOS_HEADER)mapped_file_ptr->data;

            if(dos_header->e_magic != IMAGE_DOS_SIGNATURE) {
                logger->write_info("Mapper e_mage != IMAGE_DOS_SIGNATURE");
                mapped_vec_shared->push_back(nt_header);
                continue;
            }

            if(dos_header->e_lfanew < 0) {
                logger->write_info("Mapper e_lfanew < 0");
                mapped_vec_shared->push_back(nt_header);
                continue;
            }

            headers_size = dos_header->e_lfanew + sizeof(nt_header->Signature) + sizeof(IMAGE_FILE_HEADER);

            logger->write_info("pe_file_controller<MAPPED_FILE>::get_pe_header, Step 1) header size",
                    boost::lexical_cast<std::string>(headers_size));

            if(mapped_file_ptr->size < headers_size) {
                logger->write_info("Mapper size < headers_size");
                mapped_vec_shared->push_back(nt_header);
                continue;
            }

            // completed type get header
            nt_header = (PIMAGE_NT_HEADERS)(mapped_file_ptr->data + dos_header->e_lfanew);

            headers_size += nt_header->FileHeader.SizeOfOptionalHeader;

            logger->write_info("pe_file_controller<MAPPED_FILE>::get_pe_header, header size",
                    boost::lexical_cast<std::string>(headers_size));

            logger->write_info("pe_file_controller<MAPPED_FILE>::get_pe_header,  mapped_file_ptr->size",
                    boost::lexical_cast<std::string>(mapped_file_ptr->size));

            logger->write_info_test("pe_file_controller<MAPPED_FILE>::get_pe_header, Signature",
                    boost::lexical_cast<std::string>(nt_header->Signature));

            if(nt_header->Signature == IMAGE_NT_SIGNATURE &&
                    nt_header->FileHeader.Machine == IMAGE_FILE_MACHINE_I386 &&
                    mapped_file_ptr->size > headers_size) {
                mapped_vec_shared->push_back(nt_header);

                logger->write_info_test("pe_file_controller<MAPPED_FILE>::get_pe_header, push back completed");
            }

            //test only
            //mapped_vec_shared->push_back(nt_header);

        }

        logger->write_info_test("pe_file_controller<MAPPED_FILE>::get_pe_header, return mapped_vec_shared",
                boost::lexical_cast<std::string>(mapped_vec_shared->size()));

        return *mapped_vec_shared;
    }


    template<typename MAPPED_FILE>
    uint8_t  pe_file_controller<MAPPED_FILE>::retrive_offset_lite(
            std::vector<MAPPED_FILE *>  pe_map_vec_ptr,
            std::vector<struct IMAGE_NT_HEADERS *> pe_header)const
    {
        int count_offset;
        int count_section;

        PIMAGE_SECTION_HEADER section;
        PIMAGE_NT_HEADERS  nt_header;
        struct IMAGE_NT_HEADERS_EXT *nt_headers_ext;
        MAPPED_FILE   *pe_map_ptr;

        typename std::vector<struct IMAGE_NT_HEADERS *>::iterator iter_pe_header;
        //calculate rva
        uint64_t rva_block_;
        size_t   buffer_length_;

        for(iter_pe_header = pe_header.begin();
                iter_pe_header != pe_header.end();
                ++iter_pe_header) {
            nt_header = *iter_pe_header;

            // pe_map_vec_ptr(File mapped)  gurantee size of containing equal pe_header(Header)
            pe_map_ptr = pe_map_vec_ptr.at(std::distance(pe_header.begin(), iter_pe_header));

            rva_block_     = nt_header->OptionalHeader.AddressOfEntryPoint;
            buffer_length_ = pe_map_ptr->size -((uint8_t *)&nt_header - pe_map_ptr->data);

            nt_headers_ext = new struct IMAGE_NT_HEADERS_EXT;
            nt_headers_ext->rva_block  = rva_block_;
            nt_headers_ext->size_block = buffer_length_;

            section = IMAGE_FIRST_SECTION(nt_header);
            count_offset = 0;

            while(count_offset < MIN(nt_header->FileHeader.NumberOfSections, 60)) {
                if((uint8_t *)&section -
                        (uint8_t *)&nt_header + sizeof(IMAGE_SECTION_HEADER) < pe_map_ptr->size) {
                    if(nt_headers_ext->rva_block >= section->VirtualAddress &&
                            nt_headers_ext->rva_block < section->VirtualAddress +
                            section->SizeOfRawData) {
                        uint64_t  pe_offset_start = section->PointerToRawData + \
                                (nt_headers_ext->rva_block - section->VirtualAddress);
                        nt_headers_ext->offset = pe_offset_start;
                        pe_offset_vec_shared_ptr->push_back(	nt_headers_ext );
                    }
                }

            }
        }

        if(pe_offset_vec_shared_ptr->size() == 0)
            return 0;

        return 1;
    }


    template<typename MAPPED_FILE>
    struct IMAGE_NT_HEADERS_EXT&   pe_file_controller<MAPPED_FILE>::retrive_offset(
            MAPPED_FILE  *pe_map_ptr,
            IMAGE_NT_HEADERS *pe_header)const {

        boost::shared_ptr<struct IMAGE_NT_HEADERS_EXT>
        nth_ext_shared_ptr = boost::make_shared<struct IMAGE_NT_HEADERS_EXT>();

        PIMAGE_SECTION_HEADER section;
        //PIMAGE_NT_HEADERS  nt_header = pe_header;
        struct IMAGE_NT_HEADERS_EXT *nt_headers_ext;

        int count_offset;
        int count_section;

        section = IMAGE_FIRST_SECTION(pe_header);
        count_offset = 0;


        //logger->write_info_test("pe_file_controller::retrive_offset, \
        //pe_header->OptionalHeader32.AddressOfEntryPoint",
        //       boost::lexical_cast<std::string>(pe_header->OptionalHeader.AddressOfEntryPoint));

        //calculate block
        nth_ext_shared_ptr->rva_block  = pe_header->OptionalHeader.AddressOfEntryPoint;
        nth_ext_shared_ptr->size_block = pe_map_ptr->size -((uint8_t *)pe_header - pe_map_ptr->data);

        //printf("(RVA_BLOCK, pe_header->OptionalHeader.AddressOfEntryPoint : %d \n", \
        //pe_header->OptionalHeader.AddressOfEntryPoint);


        while(count_offset < MIN(pe_header->FileHeader.NumberOfSections, 60)) {
            if((uint8_t *)section - \
                    (uint8_t *)pe_header + sizeof(IMAGE_SECTION_HEADER) < pe_map_ptr->size) {
                if(nth_ext_shared_ptr->rva_block >= section->VirtualAddress &&
                        nth_ext_shared_ptr->rva_block < section->VirtualAddress + section->SizeOfRawData) {
                    uint64_t  pe_offset_start = section->PointerToRawData +
                            (nth_ext_shared_ptr->rva_block - section->VirtualAddress);

                    logger->write_info_test("section->PointerToRawData",
                            boost::lexical_cast<std::string>(section->PointerToRawData));
                    logger->write_info_test("nth_ext_shared_ptr->rva_block",
                            boost::lexical_cast<std::string>(nth_ext_shared_ptr->rva_block));
                    logger->write_info_test("section->VirtualAddress",
                            boost::lexical_cast<std::string>(section->VirtualAddress));

                    nth_ext_shared_ptr->offset 				= pe_offset_start;
                    nth_ext_shared_ptr->data_offset   = pe_map_ptr->data;
                    nth_ext_shared_ptr->size   				= pe_map_ptr->size;

                    logger->write_info("pe_file_controller::retrive_offset, size \n",
                            boost::lexical_cast<std::string>(nth_ext_shared_ptr->size));
                    logger->write_info("pe_file_controller::retrive_offset, offset \n",
                            boost::lexical_cast<std::string>(pe_offset_start));

                    return *nth_ext_shared_ptr.get();
                }

                section++;
                count_offset++;
            } else {

                break;
            }

        }

        return *nth_ext_shared_ptr.get();
    }


    template<typename MAPPED_FILE>
    utils::scan_file_code pe_file_controller<MAPPED_FILE>::scan(std::vector<char> *symbol_vec,
            std::vector<size_t>   *state_vec,
            std::vector<uint8_t> *file_buffer_vec)
    {
        //PE_FILE_CONTROLLER call AC-DFS algorithms.
        logger->write_info("Start pe_file_controller<MAPPED_FILE>::scan...",
                hnmav_util::format_type::type_header);

        logger->write_info("PE File, size ",
                boost::lexical_cast<std::string>(file_buffer_vec->size()));
        logger->write_info("Symbol size ",
                boost::lexical_cast<std::string>(symbol_vec->size()));
        logger->write_info("State  size ",
                boost::lexical_cast<std::string>(state_vec->size()));
        logger->write_info("Send to pe_file_controller::scan, send symbol, state and binary to ocl",
                hnmav_util::format_type::type_header);
        /*
        	std::vector<uint64_t>  result_vec;
        //load open file kernel file
        load_ocl_system.set_opencl_file(*this->kernel_file_path_ptr);
        load_ocl_system.cl_load_platform();
        load_ocl_system.cl_load_memory();
        load_ocl_system.cl_process_buffer(*symbol_vec, *state_vec, *file_buffer_vec, result_vec);
        load_ocl_system.cl_build_memory();
        load_ocl_system.cl_load_commandqueue();
        load_ocl_system.cl_process_commandqueue();
        */

        return utils::infected_found;
    }

    template<typename MAPPED_FILE>
    utils::scan_file_code  pe_file_controller<MAPPED_FILE>::scan(std::vector<char> *symbol_vec,
            std::vector<size_t> *state_vec,
            std::vector<MAPPED_FILE *> *mapped_file_pe_vec,
            std::string *kernel_file_path_ptr)
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

        logger->write_info("pe_file_policy::scan_file_type(), Initial file-shm size completed.");

        f_shm_handler.initial_shm(summary_file_size);
        f_shm_handler.initial_file_shm(mapped_file_pe_vec);

        logger->write_info("pe_file_policy::scan_file_type(), Intial  file-shm data completed.");

        //Send thread pointer to pe_file_controler::scan. Binary file add to tsync before call start()
        //member function of comm_thread_buff.
        //Send vector contains symbol and state.
        //send data to OCL
        tsync.init_syncocl_workload(f_shm_handler.get_map_str_shm(),
                f_shm_handler.get_map_file_size());

        logger->write_info("pe_file_policy::scan_file_type(), Initial OCL workload completed.");

        tsync.add_load_ocl_system(&load_ocl_system,
                kernel_file_path_ptr,
                symbol_vec,
                state_vec);

        //logger->write_info("pe_file_policy::scan_file_type(), OCL workload completed.");

        //tsync.add_sig_process(symbol_vec, state_vec);

        logger->write_info("pe_file_policy::scan_file_type(), Add signature completed.");

        tsync.start_processes();

        logger->write_info("pe_file_policy::scan_file_type(), Processes completed.");

        //tsync.get_slot_ocl_result();

        //TODO : Create struct contains result from OCL.: Stuct contains result with pointer.
        //return std::vector<char>();
    }// end scan


    template<typename MAPPED_FILE>
    bool pe_file_controller<MAPPED_FILE>::convert2buffer(uint8_t   *data, size_t size)
    {

        file_buffer_vec.assign(data, data+size); //--error will be change

        if(file_buffer_vec.size() == 0) return false;

        return true;
    }


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
    std::vector<uint8_t>& pe_file_controller<MAPPED_FILE>::get_file_buffer()
    {
        return file_buffer_vec;
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
