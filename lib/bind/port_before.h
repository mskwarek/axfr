#ifndef port_before_h
#define port_before_h
#include <config.h>

struct group;           /* silence warning */
struct passwd;          /* silence warning */
struct timeval;         /* silence warning */
struct timezone;        /* silence warning */

#ifdef HAVE_SYS_TIMERS_H
#include <sys/timers.h>
#endif
#include <limits.h>


#undef WANT_IRS_GR
#undef WANT_IRS_NIS
#undef WANT_IRS_PW

#undef BSD_COMP

#define DO_PTHREADS 1
#define GETGROUPLIST_ARGS const char *name, int basegid, int *groups, int *ngroups

#define SETPWENT_VOID 1
#define SETGRENT_VOID 1





#define NET_R_END_RESULT(x) /*empty*/
#define NET_R_END_RETURN void
#undef NET_R_ENT_ARGS /*empty*/


#undef NET_R_SET_RESULT /*empty*/

#define NET_R_SET_RETURN void


#define GROUP_R_RETURN struct group *
#define GROUP_R_SET_RETURN void
#undef GROUP_R_SET_RESULT /*empty*/
#define GROUP_R_END_RETURN void
#define GROUP_R_END_RESULT(x) /*empty*/
#define GROUP_R_ARGS char *buf, int buflen
#define GROUP_R_ENT_ARGS void
#define GROUP_R_OK gptr
#define GROUP_R_BAD NULL

#define HOST_R_ARGS char *buf, size_t buflen, struct hostent **answerp, int *h_errnop
#define HOST_R_BAD ERANGE
#define HOST_R_COPY buf, buflen
#define HOST_R_COPY_ARGS char *buf, int buflen
#define HOST_R_END_RESULT(x) /*empty*/
#define HOST_R_END_RETURN void
#undef HOST_R_ENT_ARGS /*empty*/
#define HOST_R_ERRNO *h_errnop = h_errno
#define HOST_R_OK 0
#define HOST_R_RETURN int
#define HOST_R_SETANSWER 1
#undef HOST_R_SET_RESULT
#define HOST_R_SET_RETURN void
#undef HOSTENT_DATA

#define NGR_R_ARGS char *buf, size_t buflen
#define NGR_R_BAD (0)
#define NGR_R_COPY buf, buflen
#define NGR_R_COPY_ARGS NGR_R_ARGS
#define NGR_R_END_RESULT(x)  /*empty*/
#define NGR_R_END_RETURN void
#undef NGR_R_ENT_ARGS /*empty*/
#define NGR_R_OK 1
#define NGR_R_RETURN int
#undef NGR_R_SET_RESULT /*empty*/
#define NGR_R_SET_RETURN void


#define PROTO_R_ARGS char *buf, size_t buflen, struct protoent **answerp
#define PROTO_R_BAD ERANGE
#define PROTO_R_COPY buf, buflen
#define PROTO_R_COPY_ARGS char *buf, size_t buflen
#define PROTO_R_END_RESULT(x) /*empty*/
#define PROTO_R_END_RETURN void
#undef PROTO_R_ENT_ARGS /*empty*/
#define PROTO_R_OK 0
#define PROTO_R_SETANSWER 1
#define PROTO_R_RETURN int
#undef PROTO_R_SET_RESULT
#define PROTO_R_SET_RETURN void





#define PASS_R_END_RESULT(x) /*empty*/
#define PASS_R_END_RETURN void
#undef PASS_R_ENT_ARGS


#undef PASS_R_SET_RESULT /*empty*/
#define PASS_R_SET_RETURN void

#define SERV_R_ARGS char *buf, size_t buflen, struct servent **answerp
#define SERV_R_BAD ERANGE
#define SERV_R_COPY buf, buflen
#define SERV_R_COPY_ARGS char *buf, size_t buflen
#define SERV_R_END_RESULT(x) /*empty*/
#define SERV_R_END_RETURN void 
#undef SERV_R_ENT_ARGS /*empty*/
#define SERV_R_OK (0)
#define SERV_R_SETANSWER 1
#define SERV_R_RETURN int
#undef SERV_R_SET_RESULT
#define SERV_R_SET_RETURN void


#define DE_CONST(konst, var) \
        do { \
                union { const void *k; void *v; } _u; \
                _u.k = konst; \
                var = _u.v; \
        } while (0)

#define UNUSED(x) (x) = (x)

#undef NEED_SOLARIS_BITTYPES
#define ISC_SOCKLEN_T socklen_t

#ifdef __GNUC__
#define ISC_FORMAT_PRINTF(fmt, args) \
	__attribute__((__format__(__printf__, fmt, args)))
#else
#define ISC_FORMAT_PRINTF(fmt, args)
#endif

#endif
