//#include <config.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include <isc/app.h>
#include <isc/netaddr.h>
#include <isc/print.h>
#include <isc/string.h>
#include <isc/util.h>
#include <isc/task.h>

#include <dns/byaddr.h>
#include <dns/fixedname.h>
#include <dns/masterdump.h>
#include <dns/message.h>
#include <dns/name.h>
#include <dns/rdata.h>
#include <dns/rdataset.h>
#include <dns/rdatatype.h>
#include <dns/rdataclass.h>
#include <dns/result.h>

#include <dig.h>

#include "digQuery.h"
#include <dig_parser.h>

extern ISC_LIST(dig_lookup_t) lookup_list;
extern dig_serverlist_t server_list;
extern ISC_LIST(dig_searchlist_t) search_list;
extern isc_boolean_t have_ipv4, have_ipv6, specified_source,
        usesearch, qr;
extern isc_mem_t *mctx;
extern isc_task_t *global_task;
dig_lookup_t *default_lookup = NULL;
static char *batchname = NULL;
static FILE *batchfp = NULL;


parse_message_cb parse_fun = NULL;

isc_result_t printmessage(dig_query_t *query, dns_message_t *msg, isc_boolean_t headers)
{
    isc_result_t result;
    dns_messagetextflag_t flags;
    isc_buffer_t *buf = NULL;
    unsigned int len = OUTPUTBUF;
    const dns_master_style_t *style;


        style = &dns_master_style_default;

    if (query->lookup->cmdline[0] != 0) {
        query->lookup->cmdline[0]=0;
    }
    debug("printmessage(%s %s %s)", headers ? "headers" : "noheaders",
          query->lookup->comments ? "comments" : "nocomments",
          "long_form");

    flags = 0;
    if (!headers) {
        flags |= DNS_MESSAGETEXTFLAG_NOHEADERS;
        flags |= DNS_MESSAGETEXTFLAG_NOCOMMENTS;
    }
    if (!query->lookup->comments)
        flags |= DNS_MESSAGETEXTFLAG_NOCOMMENTS;

    result = ISC_R_SUCCESS;

    result = isc_buffer_allocate(mctx, &buf, len);
    check_result(result, "isc_buffer_allocate");

repopulate_buffer:

    if (result == ISC_R_NOSPACE) {
buftoosmall:
        len += OUTPUTBUF;
        isc_buffer_free(&buf);
        result = isc_buffer_allocate(mctx, &buf, len);
        if (result == ISC_R_SUCCESS)
            goto repopulate_buffer;
        else
            return (result);
     }

    if (query->lookup->section_answer) {
        result = parse_message(msg,
                                           DNS_SECTION_ANSWER,
                                           style, flags, buf, parse_fun);
        if (result == ISC_R_NOSPACE)
            goto buftoosmall;
        check_result(result, "dns_message_sectiontotext");

    }

    result = dns_message_firstname(msg, DNS_SECTION_ANSWER);
    if (result != ISC_R_SUCCESS) {
//        puts("; Transfer failed.");
        return (ISC_TRUE);
    }

//    printf("dupa\n");
//    length = snprintf(resp + length, BUF_SIZE - length, "%s\n", (char *)isc_buffer_base(buf));
//    printf("%s\n", (char *)isc_buffer_base(buf));

    isc_buffer_free(&buf);
    return (result);
}

void trying(char *frm, dig_lookup_t *lookup) {
    (void) frm;
    (void) lookup;
}

void received(int bytes, isc_sockaddr_t *from, dig_query_t *query) {
    (void) bytes;
    (void) from;
    (void) query;
}

void dighost_shutdown(void) {
    char batchline[MXNAME];
    int bargc;
    char *bargv[16];
    char *input;

    if (batchname == NULL) {
        isc_app_shutdown();
        return;
    }

    if (feof(batchfp)) {
        batchname = NULL;
        isc_app_shutdown();
        if (batchfp != stdin)
            fclose(batchfp);
        return;
    }

    if (fgets(batchline, sizeof(batchline), batchfp) != 0) {
        debug("batch line %s", batchline);
        bargc = 1;
        input = batchline;
        bargv[bargc] = next_token(&input, " \t\r\n");
        while ((bargv[bargc] != NULL) && (bargc < 14)) {
            bargc++;
            bargv[bargc] = next_token(&input, " \t\r\n");
        }

//        bargv[0] = argv0;
//
//        reorder_args(bargc, (char **)bargv);
//        parse_args(ISC_TRUE, ISC_FALSE, bargc, (char **)bargv);
        start_lookup();
    } else {
        batchname = NULL;
        if (batchfp != stdin)
            fclose(batchfp);
        isc_app_shutdown();
        return;
    }
}

void tryLookup(const char* domain, const char* const asked_ns, parse_message_cb parse_message_fun) {
    isc_result_t result;
    dig_server_t *s, *s2;
    dig_lookup_t *lookup = NULL;
//    isc_buffer_t *buf = NULL;
//    unsigned int len = OUTPUTBUF;
    dns_rdatatype_t rdtype = {0};
    isc_textregion_t tr = {0};
    dig_server_t *srv = NULL;

    parse_fun = parse_message_fun;

    ISC_LIST_INIT(lookup_list);
    ISC_LIST_INIT(server_list);
    ISC_LIST_INIT(search_list);
    isc_app_start();

    setup_libs();


    //isc_buffer_allocate(mctx, &buf, len);
    default_lookup = make_empty_lookup();;


    lookup = clone_lookup(default_lookup,
            ISC_TRUE);
    tr.base = "axfr";
    tr.length = 4;
    result = dns_rdatatype_fromtext(&rdtype,
            (isc_textregion_t *)&tr);


    srv = make_server(asked_ns);
    ISC_LIST_APPEND(lookup->my_server_list, srv, link);

    lookup->rdtype = rdtype;
    lookup->rdtypeset = ISC_TRUE;
    strncpy(lookup->textname, domain,
            sizeof(lookup->textname));
    lookup->textname[sizeof(lookup->textname)-1]=0;
    lookup->trace_root = ISC_TF(lookup->trace  ||
            lookup->ns_search_only);
    lookup->new_search = ISC_TRUE;
    ISC_LIST_APPEND(lookup_list, lookup, link);

    //parse!!!!!!!!!!!!!

    setup_system();


    result = isc_app_onrun(mctx, global_task, onrun_callback, NULL);
    check_result(result, "isc_app_onrun");
    isc_app_run();
    s = ISC_LIST_HEAD(default_lookup->my_server_list);
    while (s != NULL) {
        debug("freeing server %p belonging to %p",
              s, default_lookup);
        s2 = s;
        s = ISC_LIST_NEXT(s, link);
        ISC_LIST_DEQUEUE(default_lookup->my_server_list, s2, link);
        isc_mem_free(mctx, s2);
    }
    isc_mem_free(mctx, default_lookup);

    cancel_all();
    destroy_libs();
    isc_app_finish();
}
