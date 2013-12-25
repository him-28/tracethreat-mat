#Makefile compiles c source 
#
AM_CFLAGS   = -I$(top_srcdir)/src/rules @AM_CFLAGS@

lib_LIBRARIES = librules.a

librules_adir = $(includedir)/rules

librules_a_HEADERS = yara.h lexer.h arena.h mem.h exefiles.h exec.h filemap.h proc.h re.h re_lexer.h hex_lexer.h utils.h hex_grammar.h re_grammar.h

librules_a_SOURCES = $(librules_a_HEADERS) rules.c lexer.c arena.c mem.c libyara.c exefiles.c exec.c filemap.c proc.c re.c re_lexer.c hex_lexer.c utils.c hex_grammar.c re_grammar.c
