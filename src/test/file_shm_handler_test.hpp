/*
* Copyright 2014 MTSec, Inc.
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

/*  Titles			                                          Authors	         Date

 * - File SHM handler test by get source file from setup member
 *   at InitFileSHMHandler class.
 * - MAPPED_FILE is detail of file.
 * - File-shm is shared_memory insert binary file for supporting
 *   multithread scans malwares on file.
 *   File-shm have map structure that keys contain MD5 is filename.
 *   Value of map is binary file.
 *                                                   R.Chatsiri       28/04/2014
 */


#include "memory/file_shm_handler.hpp"
#include "filetypes/pe.hpp"

#include "utils/file_offset_handler.hpp"

#define FILE_SIZE_MULTIPLE_SHM 2

using namespace memory;
using namespace utils;

class InitFileSHMHandler : public ::testing::Test
{

    protected:

        virtual void SetUp() {
            //multiple file name

            file_name_offset[0] = "/home/chatsiri/sda1/workspacemalware/lab_malwareanalysis/3/clam_ISmsi_ext.exe";
            file_name_offset[1] = "/home/chatsiri/sda1/workspacemalware/malware_debug/vir_Win.Trojan.Zbot-15693/84612796/new_folder.exe";
            file_sig = "/home/chatsiri/Dropbox/reversing_engineer/write_sig/signature_trojan.ndb";

            for(int count_file = 0; count_file < 	FILE_SIZE_MULTIPLE_SHM; count_file++) {
                list_file_type.push_back(file_name_offset[count_file]);
                s_mapped_fpe[count_file]	 = (struct MAPPED_FILE_PE *)malloc(sizeof(s_mapped_fpe));
                mapped_file_vec.push_back(s_mapped_fpe[count_file]);
            }

        }

        const char *file_name_offset[FILE_SIZE_MULTIPLE_SHM];
        std::vector<const char*> list_file_type;
        struct MAPPED_FILE_PE *s_mapped_fpe[FILE_SIZE_MULTIPLE_SHM];
        std::vector<MAPPED_FILE_PE *> mapped_file_vec;
        const char *file_sig;
};



/**
* @brief Insert file to file-shm.
*
* @param InitFileSHMHandler
* @param insert_multiple_file
*/
TEST_F(InitFileSHMHandler, insert_multiple_file)
{


    file_offset_handler<struct common_filetype, struct MAPPED_FILE_PE>  fileoffset_h;

    EXPECT_TRUE(fileoffset_h.mapped_file(list_file_type, mapped_file_vec, fileoffset_h, file_sig));

    uint64_t sum_file_size;
    std::vector<MAPPED_FILE_PE *> *mapped_file_vec_ptr = fileoffset_h.get_mapped_file();
    typename std::vector<MAPPED_FILE_PE *>::iterator iter_mapped_file;

    for(iter_mapped_file = mapped_file_vec_ptr->begin();
            iter_mapped_file != mapped_file_vec_ptr->end();
            ++iter_mapped_file) {
        MAPPED_FILE_PE *mf_pe = *iter_mapped_file;
        unsigned char *data = mf_pe->data;
        size_t size  = mf_pe->size;
        EXPECT_GT(size,0);
        sum_file_size += size;
        std::cout<<" File summary size : " << sum_file_size
                <<", file per size : " << size
                <<", mf_pe->size : " << mf_pe->size <<std::endl;
        ASSERT_TRUE(*data != NULL);

    }

    file_shm_handler<MAPPED_FILE_PE>  f_shm_handler;
    f_shm_handler.initial_shm(sum_file_size);
    f_shm_handler.initial_file_shm(mapped_file_vec_ptr);
    f_shm_handler.delete_file_shm();
    //f_shm_handler.list_detail_shm(file_name_md5);
    //EXPECT_TRUE(fileoffset_h.unmapped_file(mapped_file_vec));

}



