#ifndef PE__HPP
#define PE__HPP


/** Header for this PE file
 *   \group_pe */

typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint32_t ULONG;

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
    WORD Magic;
    BYPE  MajorLinkerVersion;		    /**< unreliable */
    BYPE  MinorLinkerVersion;		    /**< unreliable */
    DWORD SizeOfCode;			    /**< unreliable */
    DWORD SizeOfInitializedData;		    /**< unreliable */
    DWORD SizeOfUninitializedData;		    /**< unreliable */
    DWORD AddressOfEntryPoint;
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
    BYPE  MajorLinkerVersion;		    /**< unreliable */
    BYPE  MinorLinkerVersion;		    /**< unreliable */
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
struct pe_image_section_hdr {
    BYPE Name[IMAGE_SIZEOF_SHORT_NAME];			    /**< may not end with NULL */

		union{ 
			DWORD PhysicalAddress;
			DWORD VirtualSize;
		}Misc;

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

#endif
