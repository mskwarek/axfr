import pyasn
import argparse
# Initialize module and load IP to ASN database
# the sample database can be downloaded or built - see below

parser = argparse.ArgumentParser()
parser.add_argument('--file_to_process', type=str, required=True, dest='input')
parser.add_argument('--database', type=str, required=True, dest='db')
cfg = parser.parse_args()

asndb = pyasn.pyasn(cfg.db)

with open(cfg.input, 'r') as f:
    content = f.readlines()
    for line in content:
        #print line.strip()
        print(asndb.lookup(line.strip())[0])
# should return: (15169, '8.8.8.0/24'), the origin AS, and the BGP prefix it matches

#asndb.get_as_prefixes(1128)
# returns ['130.161.0.0/16', '131.180.0.0/16', '145.94.0.0/16'], TU-Delft prefixes
