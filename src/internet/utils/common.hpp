#ifndef INTERNET_SERVICE_COMMON_HPP
#define INTERNET_SERVICE_COMMON_HPP

#include "internet/msg/packedmessage_scan_client.hpp"

namespace internet
{

        struct file_detail_scan{
            std::string  uuid;
            std::string timestamp;
            message_scan::RequestScan::RequestType type;
            std::string binary;
            message_scan::RequestScan::EncodeType  scan_type;
            message_scan::RequestScan::FileType    file_type;
        };


}

#endif /* INTERNET_SERVICE_COMMON_HPP */
