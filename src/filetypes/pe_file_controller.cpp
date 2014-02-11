#include "filetypes/pe_file_controller.hpp"
#include "boost/lexical_cast.hpp"

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

        logger->write_info("Intial PE header...");

        // PE Header conntained vector. Controller pointer by shared_ptr
        boost::shared_ptr<std::vector<struct IMAGE_NT_HEADERS *> > mapped_vec_shared
            = boost::make_shared<std::vector<struct IMAGE_NT_HEADERS * > >();
        pe_header_vec_shared.push_back(mapped_vec_shared);

        struct IMAGE_DOS_HEADER *dos_header;
        struct IMAGE_NT_HEADERS *nt_header;
        size_t headers_size = 0;
        typename std::vector<MAPPED_FILE *>::iterator  iter_mf_vec;
        MAPPED_FILE *mapped_file_ptr;

        for(iter_mf_vec = mapped_file_vec->begin(); iter_mf_vec != mapped_file_vec->end(); ++iter_mf_vec) {
            mapped_file_ptr = *iter_mf_vec;

            if(*mapped_file_ptr->data < sizeof(struct IMAGE_DOS_HEADER)) {
                logger->write_info("Mappper data < IMAGE_DOS_HEADER");
                continue;
            }

            dos_header = (struct IMAGE_DOS_HEADER *)mapped_file_ptr->data;

            if(dos_header->e_magic != IMAGE_DOS_SIGNATURE &&
                    dos_header->e_magic != IMAGE_DOS_SIGNATURE_BIG) {
                logger->write_info("Mapper e_mage != IMAGE_DOS_SIGNATURE");
                mapped_vec_shared->push_back(nt_header);
                continue;
            }

            if(dos_header->e_lfanew < 0) {
                logger->write_info("Mapper e_lfanew < 0");
                mapped_vec_shared->push_back(nt_header);
                continue;
            }

            headers_size = dos_header->e_lfanew + sizeof(nt_header) + sizeof(pe_image_file_hdr);

            if(mapped_file_ptr->size < headers_size) {
                logger->write_info("Mapper size < headers_size");
                mapped_vec_shared->push_back(nt_header);
                continue;
            }

            // completed type get header
            nt_header = (IMAGE_NT_HEADERS *)(mapped_file_ptr->data + dos_header->e_lfanew);

            headers_size += nt_header->FileHeader.SizeOfOptionalHeader;

            if(nt_header->Signature == IMAGE_NT_SIGNATURE &&
                    nt_header->FileHeader.Machine == IMAGE_FILE_MACHINE_I386 &&
                    mapped_file_ptr->size < headers_size) {
                mapped_vec_shared->push_back(nt_header);
            }
        }

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

            rva_block_     = nt_header->OptionalHeader32.AddressOfEntryPoint;
            buffer_length_ = pe_map_ptr->size -((uint8_t *)&nt_header - pe_map_ptr->data);

            nt_header->rva_block  = rva_block_;
            nt_header->size_block = buffer_length_;

            section = IMAGE_FIRST_SECTION(nt_header);
            count_offset = 0;

            while(count_offset < MIN(nt_header->FileHeader.NumberOfSections, 60)) {
                if((uint8_t *)&section -
                        (uint8_t *)&nt_header + sizeof(pe_image_section_hdr) < pe_map_ptr->size) {
                    if(nt_header->rva_block >= section->VirtualAddress &&
                            nt_header->rva_block < section->VirtualAddress +
                            section->SizeOfRawData) {
                        uint64_t  pe_offset_start = section->PointerToRawData + \
                                (nt_header->rva_block - section->VirtualAddress);
                        nt_headers_ext = new struct IMAGE_NT_HEADERS_EXT;
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
        PIMAGE_NT_HEADERS  nt_header;
        struct IMAGE_NT_HEADERS_EXT *nt_headers_ext;

        int count_offset;
        int count_section;

        section = IMAGE_FIRST_SECTION(nt_header);
        count_offset = 0;

        //calculate block
        pe_header->rva_block = nt_header->OptionalHeader32.AddressOfEntryPoint;
        pe_header->size_block = pe_map_ptr->size -((uint8_t *)nt_header - pe_map_ptr->data);

        while(count_offset < MIN(nt_header->FileHeader.NumberOfSections, 60)) {
            if((uint8_t *)section -
                    (uint8_t *)nt_header + sizeof(pe_image_section_hdr) < pe_map_ptr->size) {
                if(nt_header->rva_block >= section->VirtualAddress &&
                        nt_header->rva_block < section->VirtualAddress + section->SizeOfRawData) {
                    uint64_t  pe_offset_start = section->PointerToRawData +
                            (nt_header->rva_block - section->VirtualAddress);

                    nth_ext_shared_ptr->offset 				= pe_offset_start;
                    nth_ext_shared_ptr->data_offset   = pe_map_ptr->data;
                    nth_ext_shared_ptr->size   				= pe_map_ptr->size;

                    logger->write_info("pe_file_controller::retrive_offset, size \n",
                            boost::lexical_cast<std::string>(nth_ext_shared_ptr->size));
                    logger->write_info("pe_file_controller::retrive_offset, offset \n",
                            boost::lexical_cast<std::string>(nth_ext_shared_ptr->offset));

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
    bool pe_file_controller<MAPPED_FILE>::scan(std::vector<uint64_t> file_buffer_vec)
    {
        //PE_FILE_CONTROLLER call AC-DFS algorithms.

        return true;
    }

    template<typename MAPPED_FILE>
    bool pe_file_controller<MAPPED_FILE>::convert2buffer(uint64_t   *data, size_t size)
    {
        file_buffer_vec.assign(data, data+size);

        if(file_buffer_vec.size() == 0) return false;

        return true;
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
        struct pe_image_file_hdr *pe_file_header = &image_nt_header->FileHeader;

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
