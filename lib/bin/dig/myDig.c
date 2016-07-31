#include <stdio.h>

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

#include <dig/myDig.h>

int main()
{
	dig_server_t *srv = NULL;
	 dig_lookup_t *lookup = NULL;
	srv = make_server("onet.pl");
	ISC_LIST_APPEND(lookup->my_server_list, srv, link);

	printf("dupa\n");
	return 0;
}