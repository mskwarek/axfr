# Copyright (C) 2001  Internet Software Consortium.
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

# $Id: Makefile.in,v 1.6 2001/06/01 00:45:04 bwelling Exp $

srcdir =	.

top_srcdir =	../../..

VERSION=9.2.3


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

# Alphabetically
#MANPAGES =	lwres.3 lwres_addr_parse.3 lwres_buffer.3 \
#		lwres_buffer_add.3 lwres_buffer_back.3 lwres_buffer_clear.3 \
#		lwres_buffer_first.3 lwres_buffer_forward.3 \
#		lwres_buffer_getmem.3 lwres_buffer_getuint16.3 \
#		lwres_buffer_getuint32.3 lwres_buffer_getuint8.3 \
#		lwres_buffer_init.3 lwres_buffer_invalidate.3 \
#		lwres_buffer_putmem.3 lwres_buffer_putuint16.3 \
#		lwres_buffer_putuint32.3 lwres_buffer_putuint8.3 \
#		lwres_buffer_subtract.3 lwres_conf_clear.3 \
#		lwres_conf_get.3 lwres_conf_init.3 \
#		lwres_conf_parse.3 lwres_conf_print.3 \
#		lwres_config.3 lwres_context.3 \
#		lwres_context_allocmem.3 lwres_context_create.3 \
#		lwres_context_destroy.3 lwres_context_freemem.3 \
#		lwres_context_initserial.3 lwres_context_nextserial.3 \
#		lwres_context_sendrecv.3 lwres_endhostent.3 \
#		lwres_endhostent_r.3 lwres_freeaddrinfo.3 \
#		lwres_freehostent.3 lwres_gabn.3 \
#		lwres_gabnrequest_free.3 lwres_gabnrequest_parse.3 \
#		lwres_gabnrequest_render.3 lwres_gabnresponse_free.3 \
#		lwres_gabnresponse_parse.3 lwres_gabnresponse_render.3 \
#		lwres_gai_strerror.3 lwres_getaddrinfo.3 \
#		lwres_getaddrsbyname.3 lwres_gethostbyaddr.3 \
#		lwres_gethostbyaddr_r.3 lwres_gethostbyname.3 \
#		lwres_gethostbyname2.3 lwres_gethostbyname_r.3 \
#		lwres_gethostent.3 lwres_gethostent_r.3 \
#		lwres_getipnode.3 lwres_getipnodebyaddr.3 \
#		lwres_getipnodebyname.3 lwres_getnamebyaddr.3 \
#		lwres_getnameinfo.3 lwres_getrrsetbyname.3 \
#		lwres_gnba.3 lwres_gnbarequest_free.3 \
#		lwres_gnbarequest_parse.3 lwres_gnbarequest_render.3 \
#		lwres_gnbaresponse_free.3 lwres_gnbaresponse_parse.3 \
#		lwres_gnbaresponse_render.3 lwres_herror.3 \
#		lwres_hstrerror.3 lwres_inetntop.3 \
#		lwres_lwpacket_parseheader.3 lwres_lwpacket_renderheader.3 \
#		lwres_net_ntop.3 lwres_noop.3 \
#		lwres_nooprequest_free.3 lwres_nooprequest_parse.3 \
#		lwres_nooprequest_render.3 lwres_noopresponse_free.3 \
#		lwres_noopresponse_parse.3 lwres_noopresponse_render.3 \
#		lwres_packet.3 lwres_resutil.3 \
#		lwres_sethostent.3 lwres_sethostent_r.3 \
#		lwres_string_parse.3


MANPAGES = 	lwres.3 lwres_buffer.3 lwres_config.3 lwres_context.3	\
		lwres_gabn.3 lwres_gai_strerror.3 lwres_getaddrinfo.3			\
		lwres_gethostent.3 lwres_getipnode.3 lwres_getnameinfo.3		\
		lwres_getrrsetbyname.3 lwres_gnba.3 lwres_hstrerror.3 lwres_inetntop.3	\
		lwres_noop.3 lwres_packet.3 lwres_resutil.3

HTMLPAGES = 	lwres.html lwres_buffer.html lwres_config.html lwres_context.html	\
		lwres_gabn.html lwres_gai_strerror.html lwres_getaddrinfo.html			\
		lwres_gethostent.html lwres_getipnode.html lwres_getnameinfo.html		\
		lwres_getrrsetbyname.html lwres_gnba.html lwres_hstrerror.html lwres_inetntop.html	\
		lwres_noop.html lwres_packet.html lwres_resutil.html

MANOBJS =	${MANPAGES} ${HTMLPAGES}

doc man:: ${MANOBJS}

docclean manclean maintainer-clean::
	rm -f ${MANOBJS}

installdirs:
	$(SHELL) ${top_srcdir}/mkinstalldirs ${DESTDIR}${mandir}/man3

man3 = ${DESTDIR}${mandir}/man3

install:: installdirs
	for m in ${MANPAGES}; do ${INSTALL_DATA} ${srcdir}/$$m ${DESTDIR}${mandir}/man3; done
	rm -f ${man3}/lwres_addr_parse.3
	ln ${man3}/lwres_resutil.3 ${man3}/lwres_addr_parse.3
	rm -f ${man3}/lwres_buffer_add.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_add.3
	rm -f ${man3}/lwres_buffer_back.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_back.3
	rm -f ${man3}/lwres_buffer_clear.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_clear.3
	rm -f ${man3}/lwres_buffer_first.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_first.3
	rm -f ${man3}/lwres_buffer_forward.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_forward.3
	rm -f ${man3}/lwres_buffer_getmem.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_getmem.3
	rm -f ${man3}/lwres_buffer_getuint16.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_getuint16.3
	rm -f ${man3}/lwres_buffer_getuint32.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_getuint32.3
	rm -f ${man3}/lwres_buffer_getuint8.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_getuint8.3
	rm -f ${man3}/lwres_buffer_init.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_init.3
	rm -f ${man3}/lwres_buffer_invalidate.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_invalidate.3
	rm -f ${man3}/lwres_buffer_putmem.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_putmem.3
	rm -f ${man3}/lwres_buffer_putuint16.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_putuint16.3
	rm -f ${man3}/lwres_buffer_putuint32.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_putuint32.3
	rm -f ${man3}/lwres_buffer_putuint8.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_putuint8.3
	rm -f ${man3}/lwres_buffer_subtract.3
	ln ${man3}/lwres_buffer.3 ${man3}/lwres_buffer_subtract.3
	rm -f ${man3}/lwres_conf_clear.3
	ln ${man3}/lwres_config.3 ${man3}/lwres_conf_clear.3
	rm -f ${man3}/lwres_conf_get.3
	ln ${man3}/lwres_config.3 ${man3}/lwres_conf_get.3
	rm -f ${man3}/lwres_conf_init.3
	ln ${man3}/lwres_config.3 ${man3}/lwres_conf_init.3
	rm -f ${man3}/lwres_conf_parse.3
	ln ${man3}/lwres_config.3 ${man3}/lwres_conf_parse.3
	rm -f ${man3}/lwres_conf_print.3
	ln ${man3}/lwres_config.3 ${man3}/lwres_conf_print.3
	rm -f ${man3}/lwres_context_allocmem.3
	ln ${man3}/lwres_context.3 ${man3}/lwres_context_allocmem.3
	rm -f ${man3}/lwres_context_create.3
	ln ${man3}/lwres_context.3 ${man3}/lwres_context_create.3
	rm -f ${man3}/lwres_context_destroy.3
	ln ${man3}/lwres_context.3 ${man3}/lwres_context_destroy.3
	rm -f ${man3}/lwres_context_freemem.3
	ln ${man3}/lwres_context.3 ${man3}/lwres_context_freemem.3
	rm -f ${man3}/lwres_context_initserial.3
	ln ${man3}/lwres_context.3 ${man3}/lwres_context_initserial.3
	rm -f ${man3}/lwres_context_nextserial.3
	ln ${man3}/lwres_context.3 ${man3}/lwres_context_nextserial.3
	rm -f ${man3}/lwres_context_sendrecv.3
	ln ${man3}/lwres_context.3 ${man3}/lwres_context_sendrecv.3
	rm -f ${man3}/lwres_endhostent.3
	ln ${man3}/lwres_gethostent.3 ${man3}/lwres_endhostent.3
	rm -f ${man3}/lwres_endhostent_r.3
	ln ${man3}/lwres_gethostent.3 ${man3}/lwres_endhostent_r.3
	rm -f ${man3}/lwres_freeaddrinfo.3
	ln ${man3}/lwres_getaddrinfo.3 ${man3}/lwres_freeaddrinfo.3
	rm -f ${man3}/lwres_freehostent.3
	ln ${man3}/lwres_getipnode.3 ${man3}/lwres_freehostent.3
	rm -f ${man3}/lwres_gabnrequest_free.3
	ln ${man3}/lwres_gabn.3 ${man3}/lwres_gabnrequest_free.3
	rm -f ${man3}/lwres_gabnrequest_parse.3
	ln ${man3}/lwres_gabn.3 ${man3}/lwres_gabnrequest_parse.3
	rm -f ${man3}/lwres_gabnrequest_render.3
	ln ${man3}/lwres_gabn.3 ${man3}/lwres_gabnrequest_render.3
	rm -f ${man3}/lwres_gabnresponse_free.3
	ln ${man3}/lwres_gabn.3 ${man3}/lwres_gabnresponse_free.3
	rm -f ${man3}/lwres_gabnresponse_parse.3
	ln ${man3}/lwres_gabn.3 ${man3}/lwres_gabnresponse_parse.3
	rm -f ${man3}/lwres_gabnresponse_render.3
	ln ${man3}/lwres_gabn.3 ${man3}/lwres_gabnresponse_render.3
	rm -f ${man3}/lwres_getaddrsbyname.3
	ln ${man3}/lwres_resutil.3 ${man3}/lwres_getaddrsbyname.3
	rm -f ${man3}/lwres_gethostbyaddr.3
	ln ${man3}/lwres_gethostent.3 ${man3}/lwres_gethostbyaddr.3
	rm -f ${man3}/lwres_gethostbyaddr_r.3
	ln ${man3}/lwres_gethostent.3 ${man3}/lwres_gethostbyaddr_r.3
	rm -f ${man3}/lwres_gethostbyname.3
	ln ${man3}/lwres_gethostent.3 ${man3}/lwres_gethostbyname.3
	rm -f ${man3}/lwres_gethostbyname2.3
	ln ${man3}/lwres_gethostent.3 ${man3}/lwres_gethostbyname2.3
	rm -f ${man3}/lwres_gethostbyname_r.3
	ln ${man3}/lwres_gethostent.3 ${man3}/lwres_gethostbyname_r.3
	rm -f ${man3}/lwres_gethostent_r.3
	ln ${man3}/lwres_gethostent.3 ${man3}/lwres_gethostent_r.3
	rm -f ${man3}/lwres_getipnodebyaddr.3
	ln ${man3}/lwres_getipnode.3 ${man3}/lwres_getipnodebyaddr.3
	rm -f ${man3}/lwres_getipnodebyname.3
	ln ${man3}/lwres_getipnode.3 ${man3}/lwres_getipnodebyname.3
	rm -f ${man3}/lwres_getnamebyaddr.3
	ln ${man3}/lwres_resutil.3 ${man3}/lwres_getnamebyaddr.3
	rm -f ${man3}/lwres_gnbarequest_free.3
	ln ${man3}/lwres_gnba.3 ${man3}/lwres_gnbarequest_free.3
	rm -f ${man3}/lwres_gnbarequest_parse.3
	ln ${man3}/lwres_gnba.3 ${man3}/lwres_gnbarequest_parse.3
	rm -f ${man3}/lwres_gnbarequest_render.3
	ln ${man3}/lwres_gnba.3 ${man3}/lwres_gnbarequest_render.3
	rm -f ${man3}/lwres_gnbaresponse_free.3
	ln ${man3}/lwres_gnba.3 ${man3}/lwres_gnbaresponse_free.3
	rm -f ${man3}/lwres_gnbaresponse_parse.3
	ln ${man3}/lwres_gnba.3 ${man3}/lwres_gnbaresponse_parse.3
	rm -f ${man3}/lwres_gnbaresponse_render.3
	ln ${man3}/lwres_gnba.3 ${man3}/lwres_gnbaresponse_render.3
	rm -f ${man3}/lwres_herror.3
	ln ${man3}/lwres_hstrerror.3 ${man3}/lwres_herror.3
	rm -f ${man3}/lwres_lwpacket_parseheader.3
	ln ${man3}/lwres_packet.3 ${man3}/lwres_lwpacket_parseheader.3
	rm -f ${man3}/lwres_lwpacket_renderheader.3
	ln ${man3}/lwres_packet.3 ${man3}/lwres_lwpacket_renderheader.3
	rm -f ${man3}/lwres_net_ntop.3
	ln ${man3}/lwres_inetntop.3 ${man3}/lwres_net_ntop.3
	rm -f ${man3}/lwres_nooprequest_free.3
	ln ${man3}/lwres_noop.3 ${man3}/lwres_nooprequest_free.3
	rm -f ${man3}/lwres_nooprequest_parse.3
	ln ${man3}/lwres_noop.3 ${man3}/lwres_nooprequest_parse.3
	rm -f ${man3}/lwres_nooprequest_render.3
	ln ${man3}/lwres_noop.3 ${man3}/lwres_nooprequest_render.3
	rm -f ${man3}/lwres_noopresponse_free.3
	ln ${man3}/lwres_noop.3 ${man3}/lwres_noopresponse_free.3
	rm -f ${man3}/lwres_noopresponse_parse.3
	ln ${man3}/lwres_noop.3 ${man3}/lwres_noopresponse_parse.3
	rm -f ${man3}/lwres_noopresponse_render.3
	ln ${man3}/lwres_noop.3 ${man3}/lwres_noopresponse_render.3
	rm -f ${man3}/lwres_sethostent.3
	ln ${man3}/lwres_gethostent.3 ${man3}/lwres_sethostent.3
	rm -f ${man3}/lwres_sethostent_r.3
	ln ${man3}/lwres_gethostent.3 ${man3}/lwres_sethostent_r.3
	rm -f ${man3}/lwres_string_parse.3
	ln ${man3}/lwres_resutil.3 ${man3}/lwres_string_parse.3
