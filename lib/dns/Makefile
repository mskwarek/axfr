# Copyright (C) 1998-2001, 2003  Internet Software Consortium.
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

# $Id: Makefile.in,v 1.126.2.4 2003/10/09 06:47:59 marka Exp $

srcdir =	.

top_srcdir =	../..

VERSION=9.2.3


LIBINTERFACE = 11
LIBREVISION = 2
LIBAGE = 0


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

CINCLUDES =	-I. ${DNS_INCLUDES} ${ISC_INCLUDES}

CDEFINES =
CWARNINGS =

ISCLIBS =	../../lib/isc/libisc.a

ISCDEPLIBS =	../../lib/isc/libisc.a

LIBS =		-lnsl 

# Alphabetically

DSTOBJS =	sec/dst/dst_api.o sec/dst/dst_lib.o sec/dst/dst_parse.o \
		sec/dst/dst_result.o sec/dst/hmac_link.o sec/dst/key.o

OPENSSLOBJS =	sec/dst/openssl_link.o sec/dst/openssldh_link.o \
		sec/dst/openssldsa_link.o sec/dst/opensslrsa_link.o

GSSAPIOBJS =	sec/dst/gssapi_link.o sec/dst/gssapictx.o

# Alphabetically
OBJS =		a6.o acl.o adb.o byaddr.o \
		cache.o callbacks.o compress.o \
		db.o dbiterator.o dbtable.o diff.o dispatch.o \
		dnssec.o forward.o journal.o keytable.o \
		lib.o log.o lookup.o \
		master.o masterdump.o message.o \
		name.o ncache.o nxt.o peer.o \
		rbt.o rbtdb.o rbtdb64.o rdata.o rdatalist.o \
		rdataset.o rdatasetiter.o rdataslab.o request.o \
		resolver.o result.o rootns.o sdb.o soa.o ssu.o \
		stats.o tcpmsg.o time.o timer.o tkey.o \
		tsig.o ttl.o validator.o \
		version.o view.o xfrin.o zone.o zonekey.o zt.o \
		${OTHEROBJS} ${DSTOBJS} ${OPENSSLOBJS} ${GSSAPIOBJS}

# Alphabetically
SRCS =		a6.c acl.c adb.c byaddr.c \
		cache.c callbacks.c compress.c \
		db.c dbiterator.c dbtable.c diff.c dispatch.c \
		dnssec.c forward.c journal.c keytable.c \
		lib.c log.c lookup.c \
		master.c masterdump.c message.c \
		name.c ncache.c nxt.c peer.c \
		rbt.c rbtdb.c rbtdb64.c rdata.c rdatalist.c \
		rdataset.c rdatasetiter.c rdataslab.c request.c \
		resolver.c result.c rootns.c sdb.c soa.c ssu.c \
		stats.c tcpmsg.c time.c timer.c tkey.c \
		tsig.c ttl.c validator.c \
		version.c view.c xfrin.c zone.c zonekey.c zt.c ${OTHERSRCS}

SUBDIRS =	include sec
TARGETS =	include/dns/enumtype.h include/dns/enumclass.h \
		include/dns/rdatastruct.h timestamp

DEPENDEXTRA =	./gen -F include/dns/rdatastruct.h \
		-s ${srcdir} -d >> Makefile ;


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

version.o: version.c
	${LIBTOOL} ${CC} ${ALL_CFLAGS} \
		-DVERSION=\"${VERSION}\" \
		-DLIBINTERFACE=${LIBINTERFACE} \
		-DLIBREVISION=${LIBREVISION} \
		-DLIBAGE=${LIBAGE} \
		-c ${srcdir}/version.c

libdns.a: ${OBJS}
	${AR} ${ARFLAGS} $@ ${OBJS}
	${RANLIB} $@

libdns.la: ${OBJS}
	${LIBTOOL} --mode=link \
		${CC} ${ALL_CFLAGS} -o libdns.la -rpath ${libdir} \
		-version-info ${LIBINTERFACE}:${LIBREVISION}:${LIBAGE} \
		${OBJS} ${LIBS} ${ISCLIBS}

libdstcypto.a: ${OPENSSLOBJS}
	${AR} ${ARFLAGS} $@ ${OPENSSLOBJS}
	${RANLIB} $@

libdstcypto.la: ${OPENSSLOBJS}
	${LIBTOOL} --mode=link \
		${CC} ${ALL_CFLAGS} -o $@ -rpath ${libdir} \
		-version-info ${LIBINTERFACE}:${LIBREVISION}:${LIBAGE} \
		${OPENSSLOBJS} ${LIBS}

libdstgssapi.a: ${GSSAPIOBJS}
	${AR} ${ARFLAGS} $@ ${GSSAPIOBJS}
	${RANLIB} $@

libdstgssapi.la: ${GSSAPIOBJS}
	${LIBTOOL} --mode=link \
		${CC} ${ALL_CFLAGS} -o $@ -rpath ${libdir} \
		-version-info ${LIBINTERFACE}:${LIBREVISION}:${LIBAGE} \
		${GSSAPIOBJS} ${LIBS}

timestamp: libdns.a
	touch timestamp

installdirs:
	$(SHELL) ${top_srcdir}/mkinstalldirs ${DESTDIR}${libdir}

install:: timestamp installdirs
	${LIBTOOL} ${INSTALL_DATA} libdns.a ${DESTDIR}${libdir}

clean distclean::
	rm -f libdns.a timestamp
	rm -f gen code.h include/dns/enumtype.h include/dns/enumclass.h
	rm -f include/dns/rdatastruct.h

newrr::
	rm -f code.h include/dns/enumtype.h include/dns/enumclass.h
	rm -f include/dns/rdatastruct.h

include: include/dns/enumtype.h include/dns/enumclass.h \
	include/dns/rdatastruct.h

rdata.o: code.h

include/dns/enumtype.h: gen
	./gen -s ${srcdir} -t > $@

include/dns/enumclass.h: gen
	./gen -s ${srcdir} -c > $@

include/dns/rdatastruct.h: gen \
		${srcdir}/rdata/rdatastructpre.h \
		${srcdir}/rdata/rdatastructsuf.h
	./gen -s ${srcdir} -i \
		-P ${srcdir}/rdata/rdatastructpre.h \
		-S ${srcdir}/rdata/rdatastructsuf.h > $@

code.h:	gen
	./gen -s ${srcdir} > code.h

gen: gen.c
	${CC} ${ALL_CFLAGS} -o $@ ${srcdir}/gen.c ${LIBS}

rbtdb64.o: rbtdb.c

depend: include/dns/enumtype.h include/dns/enumclass.h \
	include/dns/rdatastruct.h code.h
subdirs: include/dns/enumtype.h include/dns/enumclass.h \
	include/dns/rdatastruct.h code.h
