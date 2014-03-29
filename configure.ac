AC_PREREQ([2.59])
AC_INIT([threadsyncocl],[0.1],[technique@hnmav.org])

#unique source file 
AC_CONFIG_SRCDIR([src/bin/tsyncocl.cpp])

#extra script build
AC_CONFIG_AUX_DIR([build-aux])

#build flags
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CXX
AC_PROG_RANLIB

AC_PROG_CC

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

LIBS="-lgtest -lpthread -lboost_system -lboost_program_options -lboost_log -lboost_filesystem -lOpenCL $LIBS"

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
#AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/threadsyncocl/Makefile])
AC_CONFIG_FILES([src/data_structure/Makefile])
AC_CONFIG_FILES([src/ocl/Makefile])
AC_CONFIG_FILES([src/wrapper/Makefile])
AC_CONFIG_FILES([src/utils/Makefile])
AC_CONFIG_FILES([src/exception/Makefile])
AC_CONFIG_FILES([src/filetypes/Makefile])
AC_CONFIG_FILES([src/memory/Makefile])
AC_CONFIG_FILES([src/scan/Makefile])
AC_CONFIG_FILES([src/rules/Makefile])
AC_CONFIG_FILES([src/bin/Makefile])
AC_CONFIG_FILES([src/test/Makefile])
#output file
AC_OUTPUT
