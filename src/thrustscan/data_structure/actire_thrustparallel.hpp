#ifndef THRUSTSCAN_ACTIRE_THRUSTPARALLEL_HPP
#define THRUSTSCAN_ACTIRE_THRUSTPARALLEL_HPP


class actire_thrustparallel{

	public:
				actire_thrustparallel();
					
				bool add_vector(std::vector<char> & binary_hex_vec,
									 std::vector<uint8_t> & state_vec,	
									 std::vector<char>    & symbol_vec);
			  bool processes();

	private:
		std::vector<char>      binary_hex_vec;
		std::vector<uint8_t>   state_vec;
		std::vector<char>      symbol_vec;
		
};

#endif /* THRUSTSCAN_ACTIRE_THRUSTPARALLEL_HPP */
