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

//internal


#include "memory/file_shm_handler.hpp"
#include "utils/get_process_id_name.hpp"

#include "utils/convert.hpp"
#include "utils/md5_hash.hpp"

#include "filetypes/pe.hpp"

namespace memory
{
    template<typename MAPPED_FILE>
    bool file_shm_handler<MAPPED_FILE>::initial_shm(uint64_t full_file_size)
    {
        // Managed_shared_memory created SHM.
        file_shm = new boostinp::managed_shared_memory(
                boostinp::create_only, utils::get_process_id_name(), full_file_size * 8);
        //map_shm_ptr = new map_shm;
    }
    /*Phase-1 :  File read to string on files-shm */
    template<typename MAPPED_FILE>
    bool file_shm_handler<MAPPED_FILE>::initial_file_shm(std::vector<MAPPED_FILE *> files_map)
    {

        //char allocator
        char_allocator  char_alloc(file_shm->get_segment_manager());
        //string allocator
        string_allocator str_alloc(file_shm->get_segment_manager());
        //vector allocator
        //binary_string_shm_vec_allocator  bs_vec_alloc(file_shm.get_segment_manager());

        //map allocator
        //map_shm_allocator  map_shm_alloc(file_shm->get_segment_manager());
			  map_str_shm_allocator  mapstr_shm_alloc(file_shm->get_segment_manager());




        binary_string_shm        *binarystr_shm[files_map.size()];
        //binary_string_shm_vec    *binarystr_shm_vec[files_map.size()];

        //initial file-shm
        //map_shm_ptr = file_shm->construct<map_shm>("file-shm")(std::less<uint64_t>(), map_shm_alloc);

        map_str_shm_ptr = file_shm->construct<map_str_shm>("file-shm")(std::less<uint64_t>(), 
					mapstr_shm_alloc);

        /*
        map_shm_shared_ptr =  boostinp::managed_shared_ptr(
        file_shm.construct<map_shm_ptr>("file-shm-shared"), file_shm);
        */
        //file from map support
        //files_buff_map::const_iterator iter_files;

        typename std::vector<MAPPED_FILE *>::iterator iter_files;

        //char *details_file_hex;

        for(iter_files = files_map.begin(); iter_files != files_map.end(); ++iter_files) {
            //index get from map
            size_t index_file =  std::distance(files_map.begin(), iter_files);

            //file struct got from iterator
            MAPPED_FILE   *mf =  *iter_files;

            //MD5 variable mallocs supported mf->data converts from MD5Hash.
            //details_file_hex = (char*)malloc(sizeof(char) * mf->size);

            // insert addresses of hex char.
            char *hex_ptr = utils::convert::byte2hexstr(mf->data, mf->size);
            addr_df_hex_vec.push_back(hex_ptr);

            /*//Conver to hex type.
            			add_df_hex_vec->push_back(detail_file_hex);

            for(int count_data = 0; count_data < mf->size(); count_data++) {
                detail_file_hex[count_data] = utils::convert::byte2hex(mf->data[count_data]);
            }
            */

            //get detail_file_hex address from vector to binary_string_shm
            //test: 7115022752065567031 
            binarystr_shm[index_file] = new binary_string_shm(char_alloc);
            *binarystr_shm[index_file] = addr_df_hex_vec[index_file];

						// Test : get hex data.
            //						binary_string_shm  * bistr = binarystr_shm[index_file];
            //						for(int count_str = 0; count_str < mf->size; count_str)
            //	std::cout<<"Data : " << *bistr <<std::endl;


            //insert string to vector
            /*
             binarystr_shm_vec[index_file] = new binary_string_shm_vec(str_alloc);
             binarystr_shm_vec[index_file]->insert(binarystr_shm_vec[index_file]->begin(),
                     mf->size,
                     *binarystr_shm[index_file]);
            */

            //encode with MD5 with mf->file_name ( filename inculded path of file)
            const uint64_t  file_name_md5 =  utils::convert::MD5Hash(mf->data, mf->size);
						//std::cout<<"File name md5 : " << file_name_md5 << std::endl;
						//std::cout<<"File name path : " << mf->file_name <<std::endl;
			
            //Key : MD5 from detail insides file.
            //Value : insert vector contains detail of hex of file to value.
            /*map_shm_ptr->insert(std::pair<const uint64_t, binary_string_shm_vec>(
                    file_name_md5, *binarystr_shm_vec[index_file])); */

            //support, value type is char
            //Key : MD5 from detail insides file.
            //Value : insert vector contains detail of hex of file to value.
            map_str_shm_ptr->insert(std::pair<const uint64_t, binary_string_shm>(
                    file_name_md5, *binarystr_shm[index_file]));

        }

    }

		template<typename MAPPED_FILE>
		bool file_shm_handler<MAPPED_FILE>::list_detail_shm(uint64_t file_name_md5){
			 
			map_str_shm::iterator iter_map_shm = map_str_shm_ptr->find(file_name_md5);
			
			 std::pair<const uint64_t, binary_string_shm> pair_map_shm = *iter_map_shm;

			 const uint64_t fn_md5 = pair_map_shm.first;
			 binary_string_shm  bistr_shm = pair_map_shm.second;

			 //std::cout<<"fn_md5:"<< fn_md5 <<std::endl;
			 //std::cout<<"str : "<< bistr_shm <<std::endl;		 
			  
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
