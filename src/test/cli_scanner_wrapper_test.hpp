#include "clibasewrapper/cliwrapper/cli_scanner_wrapper.h"


using namespace controller;
using boost::shared_ptr;
using namespace wrapper;

class CliScannerWrapper : public ::testing::Test
{


};


TEST_F(CliScannerWrapper, initialscanning)
{

    struct optstruct *opts_ = (struct optstruct *)malloc(sizeof(optstruct *));

    cli_scanner_wrapper  cli_swrapper;
 				char * file_sigdb  = "/home/chatsiri/Dropbox/reversing_engineer/write_sig/signature_trojan.ndb";
				char * file_scanpath = "/home/chatsiri/Dropbox/reversing_engineer/reversing_files_test/clam_ISmsi_int.exe";
		cli_swrapper.set_filename_path(file_scanpath);
    cli_swrapper.prepare_scandesc_wrapper(file_sigdb);
}
