#ifndef COMPILER_SIGCOMP_CONTROLLER_HPP
#define COMPILER_SIGCOMP_CONTROLLER_HPP

/*                       Titles                                          Authors                 Date
 * - Signature compiler signature file scanning.                         R.Chatsiri              19/02/2014
 * - Project use boost-spirit replace yara-rules compiler                R.Chatsiri              19/02/2014
 *
 */

namespace compiler{

/**
* @brief Default compiler behaviour 
*/
	class icompiler{
		public:
			virtual bool base_rule(std::string rules_format) = 0;
			virtual bool format_rule() = 0;	
	};

/**
* @brief Signaturecomp compiles reuls for signature file .hnm
*/
	class sigcomp_controller: public icompiler{

		public:

		private:

	};

}



#endif /* COMPILER_SIGCOMP_CONTROLLER_HPP */
