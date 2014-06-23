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

/*  Titles			                                                     Authors	          Date
 * - File mapping memory supported multithead controller          R.Chatsiri         23/03/2014
 * - File details insert to value and  Key is MD5 for map.
 * - File details convert to MD5 and send to meta data.           R.Chatsiri         25/04/2014
 */

//external
#include "boost/functional/hash.hpp"

//internal
#include "memory/file_shm_handler.hpp"
#include "utils/get_process_id_name.hpp"

#include "utils/convert.hpp"
#include "utils/md5_hash.hpp"

#include "filetypes/pe.hpp"

#define EXTENED_SIZE_SHM  64 /*Issues: Plan-00004 : Investigate SHM-Sizes should equal filel sizes.*/

namespace memory
{

		template<typename MAPPED_FILE>
		file_shm_handler<MAPPED_FILE>::file_shm_handler(){

				logger_ptr = &h_util::clutil_logging<std::string, int>:: get_instance();
        logger = logger_ptr->get();


		}

    template<typename MAPPED_FILE>
    bool file_shm_handler<MAPPED_FILE>::initial_shm(uint64_t full_file_size)
    {
        try {

            //TODO: Plant-00004 :Change to memory menagement size system.
            uint64_t shm_initial_size = full_file_size * EXTENED_SIZE_SHM;
            std::string fshm_name = "file-shm";
            boostinp::shared_memory_object::remove(fshm_name.c_str());
            // Managed_shared_memory created SHM.
            file_shm = new boostinp::managed_shared_memory(
                    boostinp::create_only, fshm_name.c_str(), shm_initial_size);

            if(file_shm->get_size() == shm_initial_size) {

                logger->write_info("file_shm_handler::initial_shm(), shm-size ",
                        boost::lexical_cast<std::string>(shm_initial_size));

                //logger->write_info("file_shm_handler::initial_shm(), processes_id ",
                 //       boost::lexical_cast<std::string>(utils::get_process_id_name()));

                return true;
            }

        } catch(boostinp::bad_alloc& ex) {
            std::cerr<< ex.what() << std::endl;
						return false;
        }

      return false;

    }
    /*Phase-1 :  File read to string on files-shm */
    template<typename MAPPED_FILE>
    bool file_shm_handler<MAPPED_FILE>::initial_file_shm(std::vector<MAPPED_FILE *> *files_map)
    {

        //char allocator
        char_allocator  char_alloc(file_shm->get_segment_manager());
        //string allocator
        string_allocator str_alloc(file_shm->get_segment_manager());
        //vector allocator

        //map allocator
        map_str_shm_allocator  mapstr_shm_alloc(file_shm->get_segment_manager());




        binary_string_shm        *binarystr_shm[files_map->size()];

        //initial file-shm
        map_str_shm_ptr = file_shm->construct<map_str_shm>("file-shm")(std::less<uint64_t>(),
                mapstr_shm_alloc);


        typename std::vector<MAPPED_FILE *>::iterator iter_files;

        //char *details_file_hex;

        for(iter_files = files_map->begin(); iter_files != files_map->end(); ++iter_files) {
            //index get from map
            size_t index_file =  std::distance(files_map->begin(), iter_files);

            //file struct got from iterator
            MAPPED_FILE   *mf =  *iter_files;

            //MD5 variable mallocs supported mf->data converts from MD5Hash.
            //details_file_hex = (char*)malloc(sizeof(char) * mf->size);
            //printf("File name : %c ", mf->file_name);
            // insert addresses of hex char.
            char *hex_ptr = utils::convert::byte2hexstr(mf->data, mf->size);
            addr_df_hex_vec.push_back(hex_ptr);

            //get detail_file_hex address from vector to binary_string_shm
            //test: 7115022752065567031
            binarystr_shm[index_file] = new binary_string_shm(char_alloc);
            *binarystr_shm[index_file] = addr_df_hex_vec[index_file];


            //encode with MD5 with mf->file_name ( filename inculded path of file)
            //Plan-00004 : const uint64_t  file_name_md5 =  utils::convert::MD5Hash(mf->data, mf->size);
            boost::hash<char*> hash_file_name;
            const uint64_t file_name_md5 =  hash_file_name(const_cast<char*>(mf->file_name.c_str()));
            file_name_md5_vec.push_back(file_name_md5);

            //Key : MD5 from detail insides file.
            //Value : insert vector contains detail of hex of file to value.
            /*map_shm_ptr->insert(std::pair<const uint64_t, binary_string_shm_vec>(
                    file_name_md5, *binarystr_shm_vec[index_file])); */

            //support, value type is char
            //Key : MD5 from detail insides file.
            //Value : insert vector contains detail of hex of file to value.
            map_str_shm_ptr->insert(std::pair<const uint64_t, binary_string_shm>(
                    file_name_md5, *binarystr_shm[index_file]));

            //insert filename and file size for calculate
            map_file_size.insert(std::make_pair(file_name_md5, mf->size));

        }

    }

    template<typename MAPPED_FILE>
    bool file_shm_handler<MAPPED_FILE>::list_detail_shm(const uint64_t *file_name_md5)
    {

        map_str_shm::iterator iter_map_shm = map_str_shm_ptr->find(*file_name_md5);

        std::pair<const uint64_t, binary_string_shm> pair_map_shm = *iter_map_shm;

        const uint64_t fn_md5 = pair_map_shm.first;
        binary_string_shm  bistr_shm = pair_map_shm.second;

				logger->write_info("file_shm_handler::list_detail_shm, list fn_md",
						boost::lexical_cast<std::string>(fn_md5));
				logger->write_info("file_shm_handler::list_detail_shm, binary_string_shm",
						boost::lexical_cast<std::string>(bistr_shm));
    }

    template<typename MAPPED_FILE>
    bool file_shm_handler<MAPPED_FILE>::delete_file_shm()
    {
        if(map_str_shm_ptr->size() ==  0)
            return true;

        //TODO : list file-shm detail deleted.
        std::vector<uint64_t>::const_iterator iter_fn_md5;

        for(iter_fn_md5 = file_name_md5_vec.begin();
                iter_fn_md5 != file_name_md5_vec.end();
                ++iter_fn_md5) {
            map_str_shm_ptr->erase(*iter_fn_md5);
        }

        file_shm->destroy_ptr(map_str_shm_ptr);
        return true;
    }


    template<typename MAPPED_FILE>
    std::vector<uint64_t> file_shm_handler<MAPPED_FILE>::get_file_name_md5()
    {
        return file_name_md5_vec;
    }

    template<typename MAPPED_FILE>
    typename file_shm_handler<MAPPED_FILE>::map_str_shm& file_shm_handler<MAPPED_FILE>::get_map_str_shm()
    {
        return *map_str_shm_ptr;
    }

    template<typename MAPPED_FILE>
    typename std::map<const uint64_t, size_t> *file_shm_handler<MAPPED_FILE>::get_map_file_size()
    {
        return &map_file_size;
    }



    /*Phase-2 : File mapped set file load to shared memory

    template<typename MAPPED_FILE>
    bool  file_shm_handler<MAPPED_FILE>::create_mapped()
    {


    }

    template<typename MAPPED_FILE>
    bool file_shm_handler<MAPPED_FILE>::set_mapped_file(const char *file_name)
    {


    }

    template<typename MAPPED_FILE>
    fdesc_shm_str  file_shm_handler<MAPPED_FILE>::file_desc()
    {


    } */


    template class file_shm_handler<MAPPED_FILE_PE>;

}

#include <boost/interprocess/detail/config_end.hpp>
