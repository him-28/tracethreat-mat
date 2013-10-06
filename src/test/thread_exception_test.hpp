#include "exception/thread_exception.hpp"

using namespace error;

TEST(failure_handler, boost_generic_category)
{
	failure_handler<ErrorController, int> fd;
	boost::system::error_code ec;
	ec = *fd.thread_notfile;

	ASSERT_EQ(ec.value(), fd.thread_notfile->value());
	
}
