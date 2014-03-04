#ifndef UTIL__CONV2HEX_HPP
#define UTIL__CONV2HEX_HPP

template<typename HexInT, typename HexOutT>
class conv2hex{

public:
		hex_vec_output conv_vec_hex(std::vector<HexInt> hex_vec_input);

private: 
		typedef std::vector<HexOutT * > hex_vec_output;
		hex_vec_output  hvec_output;
};


template<typename HexInT, typename HexOutT>
hex_vec_output vec2hex<HexInT, HexOutT>::conv_vec_hex(std::vector<HexInt> hex_vec_input){



}


#endif /* UTIL__CONV2HEX_HPP */
