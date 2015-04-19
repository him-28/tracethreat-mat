/*
* Copyright 2014 Chatsiri Rattana.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*  Titles																								Authors					 Date
 * -|-------------------|    |----------------|
 *  | scan_file_engine  | -> | scan_file_type |
 *  |-------------------|    |----------------|
 *  - Scan engine call to member function selectes scan_file_type.  
 *                                                        R.Chatsiri
 */

#include <sstream>

#include "utils/base/common.hpp"

#include "scan/file_scan_policy.hpp"

namespace policy
{

    template<typename MAPPED_FILE>
    pe_file_policy<MAPPED_FILE>::pe_file_policy()
    {

    }

    template<typename MAPPED_FILE>
    pe_file_policy<MAPPED_FILE>::~pe_file_policy() { }


    // scan_file_type member function have arguement supported multiple parameter.
    /**
    * @brief scan_file_type supports Thread building blog scanning.(TBBScan)
    *
    * @param file_type_vec File name is path scanning.
    * @param mapped_file_pe  File detail such path and binary file.
    * @param sig_shm  SHM-File not only PE.
    * @param sig_engine  Signature engine for scanning.
    * @param iactire_engine_scanner   AC-Tire engine of TBBScanning.
    */
    template<typename MAPPED_FILE>
    typename pe_file_policy<MAPPED_FILE>::threatinfo_vec_type&
    pe_file_policy<MAPPED_FILE>::
    scan_file_type(std::vector<const char *> *file_type_vec,
            std::vector<MAPPED_FILE *> *mapped_file_pe,
            threatinfo_vec_type *threatinfo_vec,
            sig_shm_type  *sig_shm,
            sig_engine_type *sig_engine,
            iactire_engine_scanner_type   *iactire_engine_scanner)
    {
        logger->write_info("Start pe_file_policy<MAPPED_FILE>::scan_file_type");
        //Check Internal or Exteranl scanning.
        bool internal_msg = false;

        if(file_type_vec->empty()) {
            logger->write_info("Start pe_file_policy<MAPPED_FILE>::scan_file_type, file empty");
            return *threatinfo_vec;
        }

        typename std::vector<MAPPED_FILE *>::iterator iter_files;

        for(iter_files = mapped_file_pe->begin();
                iter_files != mapped_file_pe->end();
                ++iter_files) {
            MAPPED_FILE *mf = *iter_files;

            //if find only one mapped_file is internal is all internal
            //External message set all is exteranl as in msg_type = EXTERNAL
            if(mf->msg_type == utils::internal_msg) {
                internal_msg = true;
                break;
            }//if check  internal_msg
        }

        // If Internal message find offset on binary file.
        //[x] Mapped file in prescanning.
        //[x] Scan header and offset file found virus on infected file.
        //-[x] Header file.
        //-[x] Offset file.
        if(internal_msg) {
            fileoffset_h.mapped_file(file_type_vec, mapped_file_pe, fileoffset_h);
            pe_layout.get_header(mapped_file_pe);
            pe_layout.get_offset(mapped_file_pe);
        }// If internal_msg

        //ACTire-Parallel with TBB
        //[x]Add Sig-SHM.
        //[x]Add File-SHM * Declares on scan() member function of pe_file_controller.
        //[x]Add AC-Tire TBB Scanning.
        threatinfo_vec = &pe_fconl.scan(mapped_file_pe,
                threatinfo_vec,
                sig_shm,
                sig_engine,
                iactire_engine_scanner);

        //Unmapped file after scan completed.
        if(internal_msg)
            fileoffset_h.unmapped_file(*mapped_file_pe);

        return *threatinfo_vec;
    }


    /**
    * @brief Scan_file_type support OCL scanning.
    *
    * @param mapped_file_pe_vec  Vector contain detail of file such binary and file_name.
    * @param sig_shm  SHM-Signature insert to OCL
    */
    template<typename MAPPED_FILE>
    typename pe_file_policy<MAPPED_FILE>::threatinfo_vec_type&
    pe_file_policy<MAPPED_FILE>::
    scan_file_type(std::vector<MAPPED_FILE *> *mapped_file_pe_vec,
            memory::signature_shm<struct memory::meta_sig,
            struct memory::meta_sig_mem> * sig_shm)
    {
        logger->write_info("pe_file_policy::scan_file_type(), multiple scanning with ocl");

        pe_fconl.set_opencl_file(*this->kernel_file_path);
        //Plan-00003 : Retrun result is array.
        pe_fconl.scan(this->node_symbol_vec,
                this->node_state_vec,
                mapped_file_pe_vec,
                this->kernel_file_path,
                sig_shm);
        //Plan-00004 : read result and match with internal arena.
    }



    template<typename MAPPED_FILE>
    bool pe_file_policy<MAPPED_FILE>::
    load_plugins_type(MAPPED_FILE   *mapped_file_pe)
    {


    }

    template<typename MAPPED_FILE>
    struct MemoryBlockContext *pe_file_policy<MAPPED_FILE>::
    entry_point_offset(struct MEMORY_BLOCK_PE *memblock) {


    }


    template<typename MAPPED_FILE>
    bool pe_file_policy<MAPPED_FILE>::set_mapped_file(MAPPED_FILE *mapped_file)
    {
        mapped_files_vec.push_back(mapped_file);
        return true;
    }

    template<typename MAPPED_FILE>
    bool pe_file_policy<MAPPED_FILE>::set_mapped_file(std::vector<MAPPED_FILE *> *mapped_file)
    {
        mapped_files_vec.insert(mapped_files_vec.begin(),
                mapped_file->begin(),
                mapped_file->end());
        return true;
    }

    template<typename MAPPED_FILE>
    std::vector<MAPPED_FILE *> *pe_file_policy<MAPPED_FILE>::get_mapped_file()
    {
        return &mapped_files_vec;
    }


    template<typename MAPPED_FILE>
    bool pe_file_policy<MAPPED_FILE>::set_file_type(std::vector<const char *> *file_type)
    {
        file_type_vec.insert(file_type_vec.begin(),
                file_type->begin(),
                file_type->end());
        return true;
    }

    template<typename MAPPED_FILE>
    std::vector<const char *> *pe_file_policy<MAPPED_FILE>::get_file_type()
    {
        logger->write_info("pe_file_policy<MAPPED_FILE>::get_file_type, file no. : ",
                boost::lexical_cast<std::string>(file_type_vec.size()));

        return &file_type_vec;
    }

    template<typename MAPPED_FILE>
    bool pe_file_policy<MAPPED_FILE>::set_threatinfo_vec(threatinfo_vec_type *threatinfo_vec)
    {
        threatinfo_vec_.insert(threatinfo_vec_.begin(),
                threatinfo_vec->begin(),
                threatinfo_vec->end());
    }

    template<typename MAPPED_FILE>
    typename pe_file_policy<MAPPED_FILE>::threatinfo_vec_type & 
		pe_file_policy<MAPPED_FILE>::get_threatinfo_vec()
    {
        return threatinfo_vec_;
    }


    template<typename MAPPED_FILE>
    struct utils::file_scan_result<MAPPED_FILE>& pe_file_policy<MAPPED_FILE>::get_result()const {

    }


    //________________________________ File Policy ___________________________________________//

    template<typename MAPPED_FILE>
    file_scan_policy<MAPPED_FILE>::file_scan_policy()
    {

    }

    /**
    * @brief Scan_flie_engine support TBBScan will invoke scan_file_type that TBBScan-engine.
    *
    * @param fcol_policy  File Policy classificaties file type.
    * @param sig_shm  SHM-Signature
    * @param sig_engine Signature Engine for scanning.
    * @param iactire_engine_scanner  ACTire Engine scanning.
    */
    template<typename MAPPED_FILE>
    //std::vector<struct utils::file_scan_result<MAPPED_FILE>* >&
    typename file_scan_policy<MAPPED_FILE>::threatinfo_vec_type&
    file_scan_policy<MAPPED_FILE>::
    scan_file_engine(file_scan_policy<MAPPED_FILE> *fcol_policy,
            sig_shm_type   *sig_shm,
            sig_engine_type *sig_engine,
            iactire_engine_scanner_type *iactire_engine_scanner)
    {
        logger->write_info("In file_scan_policy<MAPPED_FILE>::scan_file_engine");

        std::vector<MAPPED_FILE *> *mapped_file_vec =  fcol_policy->get_mapped_file();
        std::vector<const char *>  *file_type_vec   =  fcol_policy->get_file_type();
        threatinfo_vec_type        *threatinfo_vec =   &fcol_policy->get_threatinfo_vec();

        logger->write_info("file_scan_policy<MAPPED_FILE>::scan_file_engine, mapped_file on : ",
                boost::lexical_cast<std::string>(mapped_file_vec->size()));

        logger->write_info("file_scan_policy<MAPPED_FILE>::scan_file_engine, file_type-vec on : ",
                boost::lexical_cast<std::string>(file_type_vec->size()));

        logger->write_info("file_scan_policy<MAPPED_FILE>::scan_file_engine, threatinfo-vec on : ",
                boost::lexical_cast<std::string>(threatinfo_vec->size()));

        typename std::vector<MAPPED_FILE *>::iterator iter_mapped_file;
        uint8_t result_file_count = 0;
        f_col_policy = fcol_policy;
        f_col_policy->get_result();

        return f_col_policy->scan_file_type(file_type_vec,
                mapped_file_vec,
                threatinfo_vec,
                sig_shm,
                sig_engine,
                iactire_engine_scanner);


        //TODO: Pluging for scanning.
        //f_col_policy->load_plugins_type(mapp_file, pl_result);


    }

    /**
    * @brief Scan_file_engine support OCL scanning base on GPU.
    *
    * @param fcol_policy  File policy classificaties file type.
    * @param mapped_file_vec  Map contains all file detail such file-detail, binary of file.
    * @param sig_shm SHM-Signature.
    */
    template<typename MAPPED_FILE>
    //std::vector<struct utils::file_scan_result<MAPPED_FILE>* >&
    typename file_scan_policy<MAPPED_FILE>::threatinfo_vec_type&
    file_scan_policy<MAPPED_FILE>::
    scan_file_engine(file_scan_policy<MAPPED_FILE> *fcol_policy,
            std::vector<MAPPED_FILE *> *mapped_file_vec,
            memory::signature_shm<struct memory::meta_sig,
            struct memory::meta_sig_mem> * sig_shm)
    {
        logger->write_info("In file_scan_policy<MAPPED_FILE>::scan_file_engine");

        uint8_t result_file_count = 0;
        f_col_policy = fcol_policy;
        f_col_policy->get_result();

        //return scanning completed all files.
        return f_col_policy->scan_file_type(mapped_file_vec, sig_shm);

        //TODO: Pluging for scanning.
        //f_col_policy->load_plugins_type(mapp_file, pl_result);

    }


    template class pe_file_policy<struct MAPPED_FILE_PE>;

    template class file_scan_policy<struct MAPPED_FILE_PE>;

}

