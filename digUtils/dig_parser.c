//
// Created by marcin on 28.07.16.
//

#include <dig_parser.h>

#include <isc/types.h>
#include <dns/types.h>
#include <dns/masterdump.h>
#include <isc/util.h>
#include <dns/rdataset.h>
#include <string.h>
#include <dns/rdataclass.h>
#include <dns/rdatatype.h>
#include <dns/rdata.h>
#include <dns/message.h>
#include <arpa/nameser_compat.h>

#define DNS_STYLEFLAG_OMIT_OWNER	0x00010000U
#define DNS_STYLEFLAG_OMIT_TTL		0x00020000U
#define DNS_STYLEFLAG_OMIT_CLASS	0x00040000U
#define DNS_STYLEFLAG_TTL		0x00080000U
#define DNS_STYLEFLAG_REL_OWNER		0x00100000U
#define DNS_STYLEFLAG_REL_DATA		0x00200000U
#define DNS_STYLEFLAG_TRUST		0x00400000U
#define DNS_STYLEFLAG_NCACHE		0x00800000U


#define VALID_SECTION(s)	(((s) >= DNS_SECTION_ANY) \
				 && ((s) < DNS_SECTION_MAX))

#define RETERR(x) do { \
	isc_result_t _r = (x); \
	if (_r != ISC_R_SUCCESS) \
		return (_r); \
	} while (0)

static isc_result_t
rdataset_totext(dns_rdataset_t *rdataset,
                dns_name_t *owner_name,
                dns_totext_ctx_t *ctx,
                isc_boolean_t omit_final_dot,
                isc_buffer_t *target,
                parse_message_cb parse_message_fun)
{
    isc_result_t result;
    unsigned int column;
    isc_boolean_t first = ISC_TRUE;
    isc_uint32_t current_ttl;
    isc_boolean_t current_ttl_valid;
    dns_rdatatype_t type;
    response_t *res = NULL;
    response_t re;
    res = &re;

    REQUIRE(DNS_RDATASET_VALID(rdataset));

    result = dns_rdataset_first(rdataset);
    REQUIRE(result == ISC_R_SUCCESS);

    current_ttl = ctx->current_ttl;
    current_ttl_valid = ctx->current_ttl_valid;

    do {
        column = 0;

        /*
         * Owner name.
         */
        if (owner_name != NULL &&
            ! ((ctx->style.flags & DNS_STYLEFLAG_OMIT_OWNER) != 0 &&
               !first))
        {
            unsigned int name_start = target->used;
            RETERR(dns_name_totext(owner_name,
                                   omit_final_dot,
                                   target));
            column += target->used - name_start;

//            snprintf(res->name, target->used - name_start + 1, "%s", (char*)target->base+name_start);
        }

        /*
         * TTL.
         */
        if (! ((ctx->style.flags & DNS_STYLEFLAG_OMIT_TTL) != 0 &&
               current_ttl_valid &&
               rdataset->ttl == current_ttl))
        {
            char ttlbuf[64];
            isc_region_t r;
            unsigned int length;

//            INDENT_TO(ttl_column);
            length = sprintf(ttlbuf, "%u", rdataset->ttl);
            INSIST(length <= sizeof ttlbuf);
            isc_buffer_availableregion(target, &r);
            if (r.length < length)
                return (ISC_R_NOSPACE);
            memcpy(r.base, ttlbuf, length);
            isc_buffer_add(target, length);
            column += length;

            /*
             * If the $TTL directive is not in use, the TTL we
             * just printed becomes the default for subsequent RRs.
             */
            if ((ctx->style.flags & DNS_STYLEFLAG_TTL) == 0) {
                current_ttl = rdataset->ttl;
                current_ttl_valid = ISC_TRUE;
            }

            res->ttl = rdataset->ttl;

        }

        /*
         * Class.
         */
        if ((ctx->style.flags & DNS_STYLEFLAG_OMIT_CLASS) == 0 ||
            ctx->class_printed == ISC_FALSE)
        {
            unsigned int class_start;
//            INDENT_TO(class_column);
            class_start = target->used;
            result = dns_rdataclass_totext(rdataset->rdclass,
                                           target);
            if (result != ISC_R_SUCCESS)
                return (result);
            column += (target->used - class_start);
            snprintf(res->cls, target->used - class_start + 1, "%s", (char*)target->base+class_start);
        }

        /*
         * Type.
         */

        if (rdataset->type == 0) {
            type = rdataset->covers;
        } else {
            type = rdataset->type;
        }

        {
            unsigned int type_start;
//            INDENT_TO(type_column);
            type_start = target->used;
//            if (rdataset->type == 0)
//                RETERR(str_totext("\\-", target));
            result = dns_rdatatype_totext(type, target);
            if (result != ISC_R_SUCCESS)
                return (result);
            column += (target->used - type_start);
            snprintf(res->type, target->used - type_start + 1, "%s", (char*)target->base+type_start);
        }

        /*
         * Rdata.
         */
////        INDENT_TO(rdata_column);
//        if (rdataset->type == 0) {
//            if (NXDOMAIN(rdataset))
//                RETERR(str_totext(";-$NXDOMAIN\n", target));
//            else
//                RETERR(str_totext(";-$NXRRSET\n", target));
//        } else {
            unsigned int rdata_start = target->used;
            dns_rdata_t rdata = DNS_RDATA_INIT;
            isc_region_t r;

            dns_rdataset_current(rdataset, &rdata);

            RETERR(dns_rdata_tofmttext(&rdata,
                                       ctx->origin,
                                       ctx->style.flags,
                                       ctx->style.line_length -
                                       ctx->style.rdata_column,
                                       ctx->linebreak,
                                       target));
            snprintf(res->rdata, target->used - rdata_start + 1, "%s", (char*)target->base+rdata_start);
            isc_buffer_availableregion(target, &r);
            if (r.length < 1)
                return (ISC_R_NOSPACE);
            r.base[0] = '\n';
            isc_buffer_add(target, 1);
//        }

        first = ISC_FALSE;
        result = dns_rdataset_next(rdataset);
    } while (result == ISC_R_SUCCESS);

    if (result != ISC_R_NOMORE)
        return (result);

    /*
     * Update the ctx state to reflect what we just printed.
     * This is done last, only when we are sure we will return
     * success, because this function may be called multiple
     * times with increasing buffer sizes until it succeeds,
     * and failed attempts must not update the state prematurely.
     */
    ctx->class_printed = ISC_TRUE;
    ctx->current_ttl= current_ttl;
    ctx->current_ttl_valid = current_ttl_valid;

    if(NULL != parse_message_fun)
    {
        parse_message_fun(res);
    }
    return (ISC_R_SUCCESS);
}


int32_t parse_rdata(dns_name_t *owner_name,
                    dns_rdataset_t *rdataset,
                    const dns_master_style_t *style,
                    isc_buffer_t *target,
                    parse_message_cb parse_message_fun)
{

    dns_totext_ctx_t ctx;
    isc_result_t result;
    result = totext_ctx_init(style, &ctx);
    if (result != ISC_R_SUCCESS) {
        UNEXPECTED_ERROR(__FILE__, __LINE__,
                         "could not set master file style");
        return (ISC_R_UNEXPECTED);
    }

    return (rdataset_totext(rdataset, owner_name, &ctx,
                            ISC_FALSE, target, parse_message_fun));
}

int32_t parse_message(dns_message_t *msg, dns_section_t section,
                      const dns_master_style_t *style,
                      dns_messagetextflag_t flags,
                      isc_buffer_t *target,
                      parse_message_cb parse_message_fun)
{
    dns_name_t *name, empty_name;
    dns_rdataset_t *rdataset;
    isc_result_t result;

    REQUIRE(DNS_MESSAGE_VALID(msg));
    REQUIRE(target != NULL);
    REQUIRE(VALID_SECTION(section));

    if (ISC_LIST_EMPTY(msg->sections[section]))
        return (ISC_R_SUCCESS);

//    if ((flags & DNS_MESSAGETEXTFLAG_NOCOMMENTS) == 0) {
//        ADD_STRING(target, ";; ");
//        if (msg->opcode != dns_opcode_update) {
//            ADD_STRING(target, sectiontext[section]);
//        }
//        else {
//            ADD_STRING(target, updsectiontext[section]);
//        }
//        ADD_STRING(target, " SECTION:\n");
//    }

    dns_name_init(&empty_name, NULL);
    result = dns_message_firstname(msg, section);
    if (result != ISC_R_SUCCESS) {
        return (result);
    }
    do {
        name = NULL;
        dns_message_currentname(msg, section, &name);
        for (rdataset = ISC_LIST_HEAD(name->list);
             rdataset != NULL;
             rdataset = ISC_LIST_NEXT(rdataset, link)) {

            result = parse_rdata(name,
                                 rdataset,
                                 style,
                                 target,
                                 parse_message_fun);

            if (result != ISC_R_SUCCESS)
                return (result);
        }
        result = dns_message_nextname(msg, section);
    } while (result == ISC_R_SUCCESS);
//    if ((flags & DNS_MESSAGETEXTFLAG_NOHEADERS) == 0 &&
//        (flags & DNS_MESSAGETEXTFLAG_NOCOMMENTS) == 0)
//        ADD_STRING(target, "\n");
    if (result == ISC_R_NOMORE)
        result = ISC_R_SUCCESS;
    return (result);
}