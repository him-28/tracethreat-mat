#ifndef MEMORY_SIGNATURE_SHM_CONTROLLER_HPP
#define MEMORY_SIGNATURE_SHM_CONTROLLER_HPP 


/*                  Titles                                          Authors              Date
 * -Factory class initials shm-signature type                       R.Chatsiri           20/03/2014
 */

namespace memory{

template<typename SignatureTypeMemory>
class signature_shm_controller{

public:
		typename boost::shared_ptr<SignatureTypeMemory> sigtype_mem_sptr;

		signature_shm_controller(std::string & file_path): file_path_(file_pa
		bool get_shm_sigtype_file();

		bool initial_shm_sigtype(std::vector<uint8_t> symbol_vec, std::vector<size_t> state_vec);
		bool rewrite_shm_sigtype(SignatureTypeMemory * sigtype);
		bool delete_shm_sigtype(SignatureTypeMemory  * sigtype); 

private:
	 sigtype_mem_ptr mem_ptr;
	 std::string & file_path_;
};


}





#endif /* MEMORY_SIGNATURE_SHM_CONTROLLER_HPP */
