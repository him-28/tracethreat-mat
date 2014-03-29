#ifndef MEMORY_SIGNATURE_SHM_BASE_HPP
#define MEMORY_SIGNATURE_SHM_BASE_HPP
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
/*  Titles			                                 Authors	          Date
 *-Signature base structure pass data          R.Chatsiri     20/03/0214
 */


struct signature_shm_base{

//signature
struct ** malware_sig;

struct ** virus_sig;

};

struct meta_sig
{

	char * sig_hex;
	char * sig_md5;
	char * sig_name;

	char * offset;
};

struct malware_sig{
	std::vector<int8_t> symbol_vec;
	std::vector<size_t> state_vec;
	
	struct meta_sig * meta_sig_name;
};


#endif /* MEMORY_SIGNATURE_SHM_BASE_HPP */
