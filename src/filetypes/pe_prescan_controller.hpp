#ifndef FILETYPES_PE_PRESCAN_CONTROLLER_HPP
#define FILETYPES_PE_PRESCAN_CONTROLLER_HPP

#if HAVE_CONFIG_H
#include "clamav-config.h"
#endif
#define _XOPEN_SOURCE 500
#include <stdio.h>
#if HAVE_STRING_H
#include <string.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <time.h>
#include <stdarg.h>

#include "cltypes.h"
//#include "clamav.h"
#include "others.h"
#include "pe.h"
#include "petite.h"
#include "fsg.h"
#include "spin.h"
#include "upx.h"
#include "yc.h"
#include "aspack.h"
#include "wwunpack.h"
#include "unsp.h"
#include "scanners.h"
#include "str.h"
#include "execs.h"
#include "md5.h"
#include "mew.h"
#include "upack.h"
#include "matcher.h"
#include "matcher-hash.h"
#include "disasm.h"
#include "special.h"
#include "ishield.h"
#include "asn1.h"
#include "sha1.h"


namespace filetypes
{

    struct offset_list {
        uint32_t offset;
        struct offset_list *next;
    };

    static void cli_multifree(void *f, ...)
    {
        void *ff;
        va_list ap;
        free(f);
        va_start(ap, f);

        while((ff=va_arg(ap, void *))) free(ff);

        va_end(ap);
    }

    struct vinfo_list {
        uint32_t rvas[16];
        unsigned int count;
    };


    class pe_prescan_controller
    {
        public:

            int cli_scanpe(cli_ctx *ctx);

            int cli_peheader(fmap_t *map, struct cli_exe_info *peinfo);
            int cli_checkfp_pe(cli_ctx *ctx, uint8_t *authsha1);

            uint32_t cli_rawaddr(uint32_t,
                    const struct cli_exe_section *,
                    uint16_t,
                    unsigned int *,
                    size_t,
                    uint32_t);

            void findres(uint32_t,
                    uint32_t,
                    uint32_t,
                    fmap_t *map,
                    struct cli_exe_section *,
                    uint16_t,
                    uint32_t,
                    int ( *)(void *, uint32_t, uint32_t, uint32_t, uint32_t),
                    void *);
        private:
    };



}



#endif
