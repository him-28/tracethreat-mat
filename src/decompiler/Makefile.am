# debug, optimized
AM_CXXFLAGS   = -I$(top_srcdir)/src/ @AM_CXXFLAGS@

# Make src file 
# libarries for memory(SHM).
lib_LIBRARIES = libdecompiler.a

#@install header to system
libdecompiler_adir = $(includedir)/decompiler

#Header
libdecompiler_a_HEADERS = decompiler_controller.hpp \
asm_x86/disasmx86_controller.hpp 

# library
libdecompiler_a_SOURCES = $(libdecompiler_a_HEADERS)  decompiler_controller.cpp \
asm_x86/disasmx86_controller.cpp


