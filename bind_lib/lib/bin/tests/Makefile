# Copyright (C) 1998-2002  Internet Software Consortium.
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM
# DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL
# INTERNET SOFTWARE CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING
# FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
# WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

# $Id: Makefile.in,v 1.113.2.1 2002/02/12 12:02:05 marka Exp $

srcdir =	.

top_srcdir =	../..


# Copyright (C) 1999-2001  Internet Software Consortium.
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM
# DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL
# INTERNET SOFTWARE CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING
# FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
# WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

# $Id: includes.in,v 1.15 2001/07/10 19:59:03 gson Exp $

# Search for machine-generated header files in the build tree,
# and for normal headers in the source tree (${top_srcdir}).
# We only need to look in OS-specific subdirectories for the
# latter case, because there are no machine-generated OS-specific
# headers.

ISC_INCLUDES = -I/home/marcin/mgr/bind-9.2.3/lib/isc/include \
	-I${top_srcdir}/lib/isc \
	-I${top_srcdir}/lib/isc/include \
	-I${top_srcdir}/lib/isc/unix/include \
	-I${top_srcdir}/lib/isc/nothreads/include

ISCCC_INCLUDES = -I/home/marcin/mgr/bind-9.2.3/lib/isccc/include \
       -I${top_srcdir}/lib/isccc/include

ISCCFG_INCLUDES = -I/home/marcin/mgr/bind-9.2.3/lib/isccfg/include \
       -I${top_srcdir}/lib/isccfg/include

DNS_INCLUDES = -I/home/marcin/mgr/bind-9.2.3/lib/dns/include \
	-I${top_srcdir}/lib/dns/include \
	-I${top_srcdir}/lib/dns/sec/dst/include

LWRES_INCLUDES = -I/home/marcin/mgr/bind-9.2.3/lib/lwres/include \
	-I${top_srcdir}/lib/lwres/unix/include \
	-I${top_srcdir}/lib/lwres/include

TEST_INCLUDES = \
	-I${top_srcdir}/lib/tests/include

CINCLUDES =	${DNS_INCLUDES} ${ISC_INCLUDES} ${ISCCFG_INCLUDES} \
		${LWRES_INCLUDES} ${OMAPI_INCLUDES}

CDEFINES =
CWARNINGS =

DNSLIBS =	../../lib/dns/libdns.a  
ISCLIBS =	../../lib/isc/libisc.a
ISCCFGLIBS = 	../../lib/isccfg/libisccfg.a
LWRESLIBS =	../../lib/lwres/liblwres.a

DNSDEPLIBS =	../../lib/dns/libdns.a
ISCDEPLIBS =	../../lib/isc/libisc.a
ISCCFGDEPLIBS = ../../lib/isccfg/libisccfg.a
LWRESDEPLIBS =	../../lib/lwres/liblwres.a

LIBS =		-lnsl 

SUBDIRS = db dst master mem names net rbt sockaddr tasks timers system

# A few of the test programs are built by default:
# cfg_test is needed for regenerating doc/misc/options;
# genrandom is needed by the system tests

# Alphabetically
TARGETS =	cfg_test \
		genrandom

# All the other tests are optional and not built by default.

# Alphabetically
XTARGETS =	adb_test \
		byaddr_test \
		byname_test \
		compress_test \
		db_test \
		entropy_test \
		entropy2_test \
		gxba_test \
		gxbn_test \
		hash_test \
		fsaccess_test \
		inter_test \
		journalprint \
		keyboard_test \
		lex_test \
		lfsr_test \
		log_test \
		lwres_test \
		lwresconf_test \
		master_test \
		mempool_test \
		name_test \
		nxtify \
		ratelimiter_test \
		rbt_test \
		rdata_test \
		rwlock_test \
		serial_test \
		shutdown_test \
		sig0_test \
		sock_test \
		sym_test \
		task_test \
		timer_test \
		wire_test \
		zone_test

# Alphabetically
SRCS =		adb_test.c \
		byaddr_test.c \
		byname_test.c \
		compress_test.c \
		db_test.c \
		entropy_test.c \
		entropy2_test.c \
		gxba_test.c \
		gxbn_test.c \
		hash_test.c \
		fsaccess_test.c \
		inter_test.c \
		journalprint.c \
		keyboard_test.c \
		lex_test.c \
		lfsr_test.c \
		log_test.c \
		lwres_test.c \
		lwresconf_test.c \
		master_test.c \
		mempool_test.c \
		name_test.c \
		nxtify.c \
		printmsg.c \
		ratelimiter_test.c \
		rbt_test.c \
		rdata_test.c \
		rwlock_test.c \
		serial_test.c \
		shutdown_test.c \
		sig0_test.c \
		sock_test.c \
		sym_test.c \
		task_test.c \
		timer_test.c \
		wire_test.c \
		zone_test.c


# Copyright (C) 1998-2003  Internet Software Consortium.
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM
# DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL
# INTERNET SOFTWARE CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING
# FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
# WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

# $Id: rules.in,v 1.40.2.5 2003/05/12 06:39:54 marka Exp $

###
### Common Makefile rules for BIND 9.
###

###
### Paths
###
### Note: paths that vary by Makefile MUST NOT be listed
### here, or they won't get expanded correctly.

prefix =	/usr/local
exec_prefix =	${prefix}
bindir =	${exec_prefix}/bin
sbindir =	${exec_prefix}/sbin
includedir =	${prefix}/include
libdir =	${exec_prefix}/lib
sysconfdir =	/etc
localstatedir =	/var
mandir =	${prefix}/man

DESTDIR =



top_builddir =	/home/marcin/mgr/bind-9.2.3

###
### All
###
### Makefile may define:
###	TARGETS

all: subdirs ${TARGETS}

###
### Subdirectories
###
### Makefile may define:
###	SUBDIRS

ALL_SUBDIRS = ${SUBDIRS} nulldir

#
# We use a single-colon rule so that additional dependencies of
# subdirectories can be specified after the inclusion of this file.
# The "depend" target is treated the same way.
#
subdirs:
	@for i in ${ALL_SUBDIRS}; do \
		if [ "$$i" != "nulldir" -a -d $$i ]; then \
			echo "making all in `pwd`/$$i"; \
			(cd $$i; ${MAKE} ${MAKEDEFS} DESTDIR="${DESTDIR}" all) || exit 1; \
		fi; \
	done

install:: all

install clean distclean maintainer-clean doc docclean man manclean::
	@for i in ${ALL_SUBDIRS}; do \
		if [ "$$i" != "nulldir" -a -d $$i ]; then \
			echo "making $@ in `pwd`/$$i"; \
			(cd $$i; ${MAKE} ${MAKEDEFS} DESTDIR="${DESTDIR}" $@) || exit 1; \
		fi; \
	done

###
### C Programs
###
### Makefile must define
###	CC
### Makefile may define
###	CFLAGS
###	CINCLUDES
###	CDEFINES
###	CWARNINGS
### User may define externally
###     EXT_CFLAGS

CC = 		gcc
CFLAGS =	-g -O2
STD_CINCLUDES =	
STD_CDEFINES =	
STD_CWARNINGS =	 -W -Wall -Wmissing-prototypes -Wcast-qual -Wwrite-strings -Wformat

.SUFFIXES:
.SUFFIXES: .c .o

ALWAYS_INCLUDES = -I${top_builddir}
ALWAYS_DEFINES = 
ALWAYS_WARNINGS =

ALL_CPPFLAGS = \
	${ALWAYS_INCLUDES} ${CINCLUDES} ${STD_CINCLUDES} \
	${ALWAYS_DEFINES} ${CDEFINES} ${STD_CDEFINES}

ALL_CFLAGS = ${EXT_CFLAGS} ${CFLAGS} \
	${ALL_CPPFLAGS} \
	${ALWAYS_WARNINGS} ${STD_CWARNINGS} ${CWARNINGS}

.c.o:
	${LIBTOOL} ${CC} ${ALL_CFLAGS} -c $<

SHELL = /bin/bash
LIBTOOL = 
PURIFY = 

MKDEP = ${SHELL} ${top_builddir}/make/mkdep

cleandir: distclean
superclean: maintainer-clean

clean distclean maintainer-clean::
	rm -f *.o *.lo *.la core *.core .depend
	rm -rf .libs

distclean maintainer-clean::
	rm -f Makefile

depend:
	@for i in ${ALL_SUBDIRS}; do \
		if [ "$$i" != "nulldir" -a -d $$i ]; then \
			echo "making depend in `pwd`/$$i"; \
			(cd $$i; ${MAKE} ${MAKEDEFS} DESTDIR="${DESTDIR}" $@) || exit 1; \
		fi; \
	done
	@if [ X"${SRCS}" != X -a X"${PSRCS}" != X ] ; then \
		echo ${MKDEP} ${ALL_CPPFLAGS} ${SRCS}; \
		${MKDEP} ${ALL_CPPFLAGS} ${SRCS}; \
		echo ${MKDEP} -ap ${ALL_CPPFLAGS} ${PSRCS}; \
		${MKDEP} -ap ${ALL_CPPFLAGS} ${PSRCS}; \
		${DEPENDEXTRA} \
	elif [ X"${SRCS}" != X ] ; then \
		echo ${MKDEP} ${ALL_CPPFLAGS} ${SRCS}; \
		${MKDEP} ${ALL_CPPFLAGS} ${SRCS}; \
		${DEPENDEXTRA} \
	elif [ X"${PSRCS}" != X ] ; then \
		echo ${MKDEP} ${ALL_CPPFLAGS} ${PSRCS}; \
		${MKDEP} -p ${ALL_CPPFLAGS} ${PSRCS}; \
		${DEPENDEXTRA} \
	fi

FORCE:

###
### Libraries
###

AR =		/usr/bin/ar
ARFLAGS =	cruv
RANLIB =	ranlib

###
### Installation
###

INSTALL =		/usr/bin/install -c
INSTALL_PROGRAM =	${INSTALL}
INSTALL_SCRIPT =	${INSTALL}
INSTALL_DATA =		${INSTALL} -m 644

###
### DocBook -> HTML
### DocBook -> man page
###

.SUFFIXES: .docbook .html .1 .2 .3 .4 .5 .6 .7 .8

OPENJADE = openjade
SGMLCATALOG = catalog
HTMLSTYLE = html/docbook.dsl
XMLDCL = docbook/dsssl/modular/dtds/decls/xml.dcl
DOCBOOK2MANSPEC = docbook2X/docbook2man-spec.pl
JADETEX = jadetex
PDFJADETEX = pdfjadetex

ONSGMLS = onsgmls
SGMLSPL = sgmlspl

#
# Note: this rule assumes the docbook.dsl stylesheet
# is being used.  If another stylesheet is used, the
# filename 'r1.htm' in the rule might have to be
# be changed.
#
.docbook.html:
	${OPENJADE} -c ${SGMLCATALOG} -t sgml -d ${HTMLSTYLE} $<
	echo "" >> r1.htm
	cat ${top_srcdir}/docutil/HTML_COPYRIGHT r1.htm > $@

.docbook.1:
	sh ${top_srcdir}/docutil/docbook2man-wrapper.sh ${top_srcdir} $< $@
.docbook.2:
	sh ${top_srcdir}/docutil/docbook2man-wrapper.sh ${top_srcdir} $< $@
.docbook.3:
	sh ${top_srcdir}/docutil/docbook2man-wrapper.sh ${top_srcdir} $< $@
.docbook.4:
	sh ${top_srcdir}/docutil/docbook2man-wrapper.sh ${top_srcdir} $< $@
.docbook.5:
	sh ${top_srcdir}/docutil/docbook2man-wrapper.sh ${top_srcdir} $< $@
.docbook.6:
	sh ${top_srcdir}/docutil/docbook2man-wrapper.sh ${top_srcdir} $< $@
.docbook.7:
	sh ${top_srcdir}/docutil/docbook2man-wrapper.sh ${top_srcdir} $< $@
.docbook.8:
	sh ${top_srcdir}/docutil/docbook2man-wrapper.sh ${top_srcdir} $< $@

all_tests: ${XTARGETS}

genrandom: genrandom.o
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ genrandom.o

adb_test: adb_test.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ adb_test.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

nxtify: nxtify.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ nxtify.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

byaddr_test: byaddr_test.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ byaddr_test.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

byname_test: byname_test.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ byname_test.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

lex_test: lex_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ lex_test.o \
		${ISCLIBS} ${LIBS}

lfsr_test: lfsr_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ lfsr_test.o \
		${ISCLIBS} ${LIBS}

log_test: log_test.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ log_test.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

name_test: name_test.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ name_test.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

hash_test: hash_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ hash_test.o \
		${ISCLIBS} ${LIBS}

entropy_test: entropy_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ entropy_test.o \
		${ISCLIBS} ${LIBS}

entropy2_test: entropy2_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ entropy2_test.o \
		${ISCLIBS} ${LIBS}

sock_test: sock_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ sock_test.o \
		${ISCLIBS} ${LIBS}

sym_test: sym_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ sym_test.o \
		${ISCLIBS} ${LIBS}

task_test: task_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ task_test.o \
		${ISCLIBS} ${LIBS}

shutdown_test: shutdown_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ shutdown_test.o \
		${ISCLIBS} ${LIBS}

timer_test: timer_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ timer_test.o \
		${ISCLIBS} ${LIBS}

ratelimiter_test: ratelimiter_test.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ ratelimiter_test.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

rbt_test: rbt_test.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ rbt_test.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

rdata_test: rdata_test.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ rdata_test.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

rwlock_test: rwlock_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ rwlock_test.o \
		${ISCLIBS} ${LIBS}

wire_test: wire_test.o printmsg.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ wire_test.o printmsg.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

master_test: master_test.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ master_test.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

db_test: db_test.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ db_test.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

compress_test: compress_test.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ compress_test.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

mempool_test: mempool_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ mempool_test.o \
		${ISCLIBS} ${LIBS}

serial_test: serial_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ serial_test.o \
		${ISCLIBS} ${LIBS}

zone_test: zone_test.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ zone_test.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

fsaccess_test: fsaccess_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ fsaccess_test.o \
		${ISCLIBS} ${LIBS}

inter_test: inter_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ inter_test.o \
		${ISCLIBS} ${LIBS}

keyboard_test: keyboard_test.o ${ISCDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ keyboard_test.o \
		${ISCLIBS} ${LIBS}

lwresconf_test: lwresconf_test.o ${ISCDEPLIBS} ${LWRESDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ lwresconf_test.o \
		${LWRESLIBS} ${ISCLIBS} ${LIBS}

lwres_test: lwres_test.o ${ISCDEPLIBS} ${LWRESDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ lwres_test.o \
		${LWRESLIBS} ${ISCLIBS} ${LIBS}

gxbn_test: gxbn_test.o ${LWRESDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ gxbn_test.o \
		${LWRESLIBS} ${ISCLIBS} ${LIBS}

gxba_test: gxba_test.o ${LWRESDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ gxba_test.o \
		${LWRESLIBS} ${ISCLIBS} ${LIBS}

sig0_test: sig0_test.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ sig0_test.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

journalprint: journalprint.o ${ISCDEPLIBS} ${DNSDEPLIBS}
	${LIBTOOL} ${PURIFY} ${CC} ${CFLAGS} -o $@ journalprint.o \
		${DNSLIBS} ${ISCLIBS} ${LIBS}

cfg_test: cfg_test.o ${ISCCFGDEPLIBS} ${DNSDEPLIBS} ${ISCDEPLIBS}
	${LIBTOOL} ${CC} ${CFLAGS} -o $@ cfg_test.o \
		${ISCCFGLIBS} ${DNSLIBS} ${ISCLIBS} ${LIBS}

distclean::
	rm -f headerdep_test.sh

clean distclean::
	rm -f ${TARGETS} ${XTARGETS}
	rm -f t_journal

test:
	@for dir in $(SUBDIRS) ;\
	do \
		( cd $$dir; $(MAKE) test ) ;\
	done
