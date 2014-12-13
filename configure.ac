AC_INIT([hnmav], [0.1], [rchatsiri@hnmav.io], [avanalysis-tools], [http://www.hnmav.io])

#unique source file 
AC_CONFIG_SRCDIR([src/bin/tsyncocl.cpp])

#extra script build
AC_CONFIG_AUX_DIR([build-aux])

#build flags
AM_INIT_AUTOMAKE([-Wall -Werror foreign])


AC_PROG_CXX
AC_PROG_RANLIB

#AC_PROG_CC

#check for program
AC_LANG([C++])
#CFLAGS_CXX=-std=c++0x

#check header file
AC_HEADER_STDC
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([iostream])


#libtools
#m4_pattern_allow([AC_PROG_LIBTOOL])
#AC_PROG_LIBTOOL

LIBS="-lgtest -lglog -lboost_thread -lpthread -lboost_system -lboost_program_options -lboost_log -lboost_filesystem -lboost_timer -lOpenCL -lrt -lclamav -lprotobuf -ldl -lfolly -lmongoclient -lrocksdb $LIBS"

AC_TYPE_SIZE_T
##################################
##   Libraries declare path     ##
##################################

#check for header files.
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])
AC_SUBST([LIBS])

AC_SUBST([AM_CFLAGS])


#check library function
AC_CONFIG_HEADERS([config.h])

#AC_CONFIG_FILES([src/wrapper/Makefile])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/threadsyncocl/Makefile])
AC_CONFIG_FILES([src/data_structure/Makefile])
AC_CONFIG_FILES([src/ocl/Makefile])
AC_CONFIG_FILES([src/utils/Makefile])
AC_CONFIG_FILES([src/exception/Makefile])
AC_CONFIG_FILES([src/filetypes/Makefile])
AC_CONFIG_FILES([src/memory/Makefile])
AC_CONFIG_FILES([src/scan/Makefile])
AC_CONFIG_FILES([src/bin/Makefile])
AC_CONFIG_FILES([src/decompiler/Makefile])
AC_CONFIG_FILES([src/clibasewrapper/Makefile])
AC_CONFIG_FILES([src/threadconcurrency/Makefile])
AC_CONFIG_FILES([src/tbbscan/Makefile])
AC_CONFIG_FILES([src/filestructure/Makefile])
AC_CONFIG_FILES([src/filestructure/test_fs/Makefile])
AC_CONFIG_FILES([src/gensign/Makefile])
AC_CONFIG_FILES([src/gensign/testgensig/Makefile])
AC_CONFIG_FILES([src/internet/client_fresh/Makefile])
AC_CONFIG_FILES([src/internet/scan_server/Makefile])
AC_CONFIG_FILES([src/internet/scan_client/Makefile])
AC_CONFIG_FILES([src/internet/test_server/Makefile])
AC_CONFIG_FILES([src/internet/test_client/Makefile])
AC_CONFIG_FILES([src/test/Makefile])
#output file
AC_OUTPUT
