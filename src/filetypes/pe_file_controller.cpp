#include "filetypes/pe_file_controller.hpp"

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
    boost::shared_ptr<std::vector<IMAGE_NT_HEADERS *> >&
    pe_file_controller<MAPPED_FILE>::get_pe_header(std::vector<MAPPED_FILE *> *mapped_file_vec)
    {

        logger->write_info("Intial PE header...");

        boost::shared_ptr<std::vector<IMAGE_NT_HEADERS *> > mapped_vec_shared
            = boost::make_shared<std::vector<IMAGE_NT_HEADERS * > >();
        pe_header_vec_shared.push_back(mapped_vec_shared);

        struct IMAGE_DOS_HEADER *dos_header;
        struct IMAGE_NT_HEADERS *nt_header;
        size_t headers_size = 0;
        typename std::vector<MAPPED_FILE *>::iterator  iter_mf_vec;
        MAPPED_FILE *mapped_file_ptr;

        for(iter_mf_vec = mapped_file_vec->begin(); iter_mf_vec != mapped_file_vec->end(); ++iter_mf_vec) {
            mapped_file_ptr = *iter_mf_vec;

            if(*mapped_file_ptr->data < sizeof(struct IMAGE_DOS_HEADER))
                continue;

            dos_header = (struct IMAGE_DOS_HEADER *)mapped_file_ptr->data;

            if(dos_header->e_magic != IMAGE_DOS_SIGNATURE)
                continue;

            if(dos_header->e_lfanew < 0)
                continue;

            headers_size = dos_header->e_lfanew + sizeof(nt_header) + sizeof(pe_image_file_hdr);

            if(mapped_file_ptr->size < headers_size)
                continue;

            nt_header = (IMAGE_NT_HEADERS *)(mapped_file_ptr->data + dos_header->e_lfanew);

            headers_size += nt_header->FileHeader.SizeOfOptionalHeader;

            if(nt_header->Signature == IMAGE_NT_SIGNATURE &&
                    nt_header->FileHeader.Machine == IMAGE_FILE_MACHINE_I386 &&
                    mapped_file_ptr->size < headers_size) {
                mapped_vec_shared->push_back(nt_header);
            }
        }

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
        struct pe_image_file_hdr *pe_file_header = image_nt_header->FileHeader;

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
            logger->write_info("Machine type: ** UNKNOWN ** (0x%x)\n", EC16(pe_file_header->Machine));
        }
    }



}
