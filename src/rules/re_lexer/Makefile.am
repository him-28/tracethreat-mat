#Makefile compiles c source 
#
AM_CFLAGS   =  -I$(top_srcdir)/src/rules/  -I$(top_srcdir)/src/rules/re_lexer @AM_CFLAGS@

lib_LIBRARIES = librelexer.a

librelexer_adir = $(includedir)/rules/re_lexer

librelexer_a_HEADERS = re_lexer.h

librelexer_a_SOURCES = $(librelexer_a_HEADERS) re_lexer.c
