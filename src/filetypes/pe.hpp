#ifndef PE__HPP
#define PE__HPP

#include "stdint.h"
#include "stdio.h"

#define FILE_DESCRIPTOR  int

/** Header for this PE file
 *   \group_pe */

typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint32_t ULONG;


#define FIELD_OFFSET(type, field)    ((size_t)&(((type *)0)->field))

#define IMAGE_DOS_SIGNATURE                 0x4D5A      // MZ
#define IMAGE_OS2_SIGNATURE                 0x4E45      // NE
#define IMAGE_OS2_SIGNATURE_LE              0x4C45      // LE
#define IMAGE_NT_SIGNATURE                  0x50450000  // PE00

#define IMAGE_FIRST_SECTION( ntheader ) ((PIMAGE_SECTION_HEADER)        \
    ((BYTE*)ntheader +                                              \
     FIELD_OFFSET( IMAGE_NT_HEADERS, OptionalHeader ) +                 \
     ((PIMAGE_NT_HEADERS)(ntheader))->FileHeader.SizeOfOptionalHeader   \
    ))


#define IMAGE_SIZEOF_FILE_HEADER             20


#define IMAGE_FILE_RELOCS_STRIPPED           0x0001  // Relocation info stripped from file.
#define IMAGE_FILE_EXECUTABLE_IMAGE          0x0002  // File is executable  (i.e. no unresolved externel references).
#define IMAGE_FILE_LINE_NUMS_STRIPPED        0x0004  // Line nunbers stripped from file.
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED       0x0008  // Local symbols stripped from file.
#define IMAGE_FILE_AGGRESIVE_WS_TRIM         0x0010  // Agressively trim working set
#define IMAGE_FILE_LARGE_ADDRESS_AWARE       0x0020  // App can handle >2gb addresses
#define IMAGE_FILE_BYTES_REVERSED_LO         0x0080  // Bytes of machine word are reversed.
#define IMAGE_FILE_32BIT_MACHINE             0x0100  // 32 bit word machine.
#define IMAGE_FILE_DEBUG_STRIPPED            0x0200  // Debugging info stripped from file in .DBG file
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP   0x0400  // If Image is on removable media, copy and run from the swap file.
#define IMAGE_FILE_NET_RUN_FROM_SWAP         0x0800  // If Image is on Net, copy and run from the swap file.
#define IMAGE_FILE_SYSTEM                    0x1000  // System File.
#define IMAGE_FILE_DLL                       0x2000  // File is a DLL.
#define IMAGE_FILE_UP_SYSTEM_ONLY            0x4000  // File should only be run on a UP machine
#define IMAGE_FILE_BYTES_REVERSED_HI         0x8000  // Bytes of machine word are reversed.


#define IMAGE_FILE_MACHINE_I386              0x014c  // Intel 386.

#define IMAGE_SUBSYSTEM_UNKNOWN              0   // Unknown subsystem.
#define IMAGE_SUBSYSTEM_NATIVE               1   // Image doesn't require a subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_GUI          2   // Image runs in the Windows GUI subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_CUI          3   // Image runs in the Windows character subsystem.
#define IMAGE_SUBSYSTEM_OS2_CUI              5   // image runs in the OS/2 character subsystem.
#define IMAGE_SUBSYSTEM_POSIX_CUI            7   // image runs in the Posix character subsystem.
#define IMAGE_SUBSYSTEM_NATIVE_WINDOWS       8   // image is a native Win9x driver.




struct IMAGE_DOS_HEADER {      // DOS .EXE header
    WORD   e_magic;                     // Magic number
    WORD   e_cblp;                      // Bytes on last page of file
    WORD   e_cp;                        // Pages in file
    WORD   e_crlc;                      // Relocations
    WORD   e_cparhdr;                   // Size of header in paragraphs
    WORD   e_minalloc;                  // Minimum extra paragraphs needed
    WORD   e_maxalloc;                  // Maximum extra paragraphs needed
    WORD   e_ss;                        // Initial (relative) SS value
    WORD   e_sp;                        // Initial SP value
    WORD   e_csum;                      // Checksum
    WORD   e_ip;                        // Initial IP value
    WORD   e_cs;                        // Initial (relative) CS value
    WORD   e_lfarlc;                    // File address of relocation table
    WORD   e_ovno;                      // Overlay number
    WORD   e_res[4];                    // Reserved words
    WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
    WORD   e_oeminfo;                   // OEM information; e_oemid specific
    WORD   e_res2[10];                  // Reserved words
    LONG   e_lfanew;                    // File address of new exe header
  };


struct pe_image_file_hdr {
    DWORD Magic;  /**< PE magic header: PE\\0\\0 */
    WORD Machine;/**< CPU this executable runs on, see libclamav/pe.c for possible values */
    WORD NumberOfSections;/**< Number of sections in this executable */
    DWORD TimeDateStamp;   /**< Unreliable */
    DWORD PointerToSymbolTable;	    /**< debug */
    DWORD NumberOfSymbols;		    /**< debug */
    WORD SizeOfOptionalHeader;	    /**< == 224 */
    WORD Characteristics;
};

/** PE data directory header
 *   \group_pe */
struct pe_image_data_dir {
    DWORD VirtualAddress;
    DWORD Size;
};

/** 32-bit PE optional header
 *   \group_pe */
struct pe_image_optional_hdr32 {
		// Standard field.
    WORD Magic;
    BYTE  MajorLinkerVersion;		    /**< unreliable */
    BYTE  MinorLinkerVersion;		    /**< unreliable */
    DWORD SizeOfCode;			    /**< unreliable */
    DWORD SizeOfInitializedData;		    /**< unreliable */
    DWORD SizeOfUninitializedData;		    /**< unreliable */
    DWORD AddressOfEntryPoint;

		// NT additional fields.
    DWORD BaseOfCode;
    DWORD BaseOfData;
    DWORD ImageBase;				    /**< multiple of 64 KB */
    DWORD SectionAlignment;			    /**< usually 32 or 4096 */
    DWORD FileAlignment;			    /**< usually 32 or 512 */
    WORD MajorOperatingSystemVersion;	    /**< not used */
    WORD MinorOperatingSystemVersion;	    /**< not used */
    WORD MajorImageVersion;			    /**< unreliable */
    WORD MinorImageVersion;			    /**< unreliable */
    WORD MajorSubsystemVersion;
    WORD MinorSubsystemVersion;
    DWORD Win32VersionValue;			    /*< ? */
    DWORD SizeOfImage;
    DWORD SizeOfHeaders;
    DWORD CheckSum;				    /**< NT drivers only */
    WORD Subsystem;
    WORD DllCharacteristics;
    DWORD SizeOfStackReserve;
    DWORD SizeOfStackCommit;
    DWORD SizeOfHeapReserve;
    DWORD SizeOfHeapCommit;
    DWORD LoaderFlags;			    /*< ? */
    DWORD NumberOfRvaAndSizes;		    /**< unreliable */
    struct pe_image_data_dir DataDirectory[16];
};

/** PE 64-bit optional header
 *   \group_pe */
struct pe_image_optional_hdr64 {
    WORD Magic;
    BYTE  MajorLinkerVersion;		    /**< unreliable */
    BYTE  MinorLinkerVersion;		    /**< unreliable */
    DWORD SizeOfCode;			    /**< unreliable */
    DWORD SizeOfInitializedData;		    /**< unreliable */
    DWORD SizeOfUninitializedData;		    /**< unreliable */
    DWORD AddressOfEntryPoint;
    DWORD BaseOfCode;
    uint64_t ImageBase;				    /**< multiple of 64 KB */
    DWORD SectionAlignment;			    /**< usually 32 or 4096 */
    DWORD FileAlignment;			    /**< usually 32 or 512 */
    WORD MajorOperatingSystemVersion;	    /**< not used */
    WORD MinorOperatingSystemVersion;	    /**< not used */
    WORD MajorImageVersion;			    /**< unreliable */
    WORD MinorImageVersion;			    /**< unreliable */
    WORD MajorSubsystemVersion;
    WORD MinorSubsystemVersion;
    DWORD Win32VersionValue;			    /* ? */
    DWORD SizeOfImage;
    DWORD SizeOfHeaders;
    DWORD CheckSum;				    /**< NT drivers only */
    WORD Subsystem;
    WORD DllCharacteristics;
    uint64_t SizeOfStackReserve;
    uint64_t SizeOfStackCommit;
    uint64_t SizeOfHeapReserve;
    uint64_t SizeOfHeapCommit;
    DWORD LoaderFlags;			    /* ? */
    DWORD NumberOfRvaAndSizes;		    /**< unreliable */
    struct pe_image_data_dir DataDirectory[16];
};

/** PE section header
 *   \group_pe */
#define IMAGE_SIZEOF_SHORT_NAME 8

typedef struct _IMAGE_DATA_DIRECTORY{
        DWORD PhysicalAddress;
        DWORD VirtualSize;
    }MAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;


struct pe_image_section_hdr {
    BYTE Name[IMAGE_SIZEOF_SHORT_NAME];			    /**< may not end with NULL */ 
    DWORD VirtualSize;
    DWORD VirtualAddress;
    DWORD SizeOfRawData;		    /**< multiple of FileAlignment */
    DWORD PointerToRawData;		    /**< offset to the section's data */
    DWORD PointerToRelocations;	    /**< object files only */
    DWORD PointerToLinenumbers;	    /**< object files only */
    WORD NumberOfRelocations;	    /**< object files only */
    WORD NumberOfLinenumbers;	    /**< object files only */
    DWORD Characteristics;
};

/** Data for the bytecode PE hook
 *   \group_pe */
struct cli_pe_hook_data {
    DWORD offset;
    DWORD ep; /**< EntryPoint as file offset */
    WORD nsections;/**< Number of sections */
    WORD dummy; /* align */
    struct pe_image_file_hdr file_hdr;/**< Header for this PE file */
    struct pe_image_optional_hdr32 opt32; /**< 32-bit PE optional header */
    DWORD dummy2; /* align */
    struct pe_image_optional_hdr64 opt64;/**< 64-bit PE optional header */
    struct pe_image_data_dir dirs[16]; /**< PE data directory header */
    DWORD e_lfanew;/**< address of new exe header */
    DWORD overlays;/**< number of overlays */
    LONG overlays_sz;/**< size of overlays */
    DWORD hdr_size;/**< internally needed by rawaddr */
};

struct MAPPED_FILE_PE {
    FILE_DESCRIPTOR   file;
    size_t            size;
    uint8_t 					*data;
		uint32_t          ops_begin;
		uint32_t					ops_end;
};

struct MAMORY_BLOCK_PE {
    unsigned char *data;
    size_t				 	size;
    size_t					base;
};


struct IMAGE_NT_HEADERS{
DWORD Signature;
struct pe_image_file_hdr  FileHeader;
struct pe_image_optional_hdr32  OptionalHeader32;		
struct pe_image_optional_hdr64  OptionalHeader64;	

};


#endif
