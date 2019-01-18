# myDig

AXFR scanner ic C

## Test coverage

- [![C coverage]()](https://mskwarek.gitlab.io/myDig) C/C++

# Requirements

- googletest

## Build

- `git clone`
- `cd mydns`
- `mkdir build`
- `cmake ..`
- `make -j4`

## Usage

```bash
$ ./build/src/axfrScanner
        -o      output folder [mandatory]
        -t      timeout in ms [default 5s]
        -d      domain to scan [optional; scan only one domain]
        -n      NS IP address [optional; needed by one-domain-scan mode]
        -q      Query type [A, AAAA, NS, CNAME, SOA, PTR, MX, TXT, HINFO, RP, AFSDB, LOC, SRV, NAPTR,
                RRSIG, NSEC, DNSKEY, AXFR, ALL; default AXFR]
        -p      Transport protocol used in scanner [default TCP]
        -l      list with records to scan in format domain_address|ns_ip [optional]
```

To scan list of domains:

```bash
./build/src/axfrScanner -o folderThatContainsResultZones -t 3 -q ALL -p UDP -l inputList.dat
```

To scan just one domain:

```bash
./build/src/axfrScanner -o folderThatContainsResultZones -t 3 -q ALL -p UDP -d example.domain.com -n 10.10.10.1
```

You have to create output folder before scan.
