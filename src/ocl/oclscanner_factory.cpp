#ifndef KERNEL_OCLSCANNER_FACTORY_HPP
#define KERNEL_OCLSCANNER_FACTORY_HPP

#include "ocl/oclscanner.hpp"
namespace kernel{

oclscanner_factory::callback_map oclscanner_factory::m_scanners;

class oclscanner_factory{

	public:
			typedef oclscanner *(*create_callback)();
			static void regis_scanner(const std::string & type, create_callback cb);
			static void unregis_scanner(const std::string & type);
			static void oclscanner * create_scanner(const std::string & type);
	private:
		typedef std::map<std::string, create_callback> 	callback_map;
		static callback_map m_scanners;


};

void oclscanner::regis_scanner(const std::string & type, create_callback cb)
{
	m_scanners[type] = cb;
}

void oclscanner::unregis_scanner(const std::string & type)
{
		m_scanners.erase(type);
}


void oclscanner::create_scanner(const std::string & type)
{

		callback_map::iterator iter = m_scanners.find(type);
		if(iter != m_scanners.end())
		{
			return (iter->second)();
		}
}

class pe_scanner : public oclscanner{

	public:
		~pe_scanner();
		  

};

}
#endif
