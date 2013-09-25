#include <boost/system/error_code.hpp>

template<typename ErrorControl>
failure_handler<ErrorControl> & failure_handler<ErrorControl>::failure_error_category = NULL;

tempalte<typename ErrorControl>
failure_handler<ErrorControl>::failure_handler()
{
	if(failure_error_category == NULL)
			failure_error_category = new failure_error_category<ErrorControl>();
}

