
#ifndef PLUGINS_PE_FILTER_H
#define PLUGINS_PE_FILTER_H
namespace plugins
{

    class pe_filter
    {
        public:
            void corrupt_header(unsigned char *data, uint64_t   *size);
						void corrupt_offset(unsigned char *data, uint64_t   *size);
            bool get_status();
        private:
            unsigned char *data;
            uint64_t        size;
            uint64_t        base;
            bool            filter_status;
    };

}

