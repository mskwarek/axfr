import sqlite3
from string import Template


def get_template():
    return Template('''
        
        insert into url(id, address) select (select max(id)+1 from url), '$address'
            where not exists(select * from url where address='$address');
        insert into registrar(id, name) select (select max(id)+1 from registrar), '$registrar'
            where not exists(select * from registrar where name='$registrar');
        
        insert into address_to_registrar(add_id, registrar_id) select (select id from url where address='$address'), (select id from registrar where name='$registrar')
            where not exists(select * from address_to_registrar where add_id=(select id from url where address='$address') and registrar_id=(select id from registrar where name='$registrar')); 
    ''')

def insert_values(cur, address, registrar):
    cur.executescript(get_template().substitute(address=address, registrar=registrar))

def __main__():
    with open('/home/mkoi/mgr_wyniki/ip.input') as f:
        content = f.readlines()
    # you may also want to remove whitespace characters like `\n` at the end of each line
    content = [x.strip() for x in content]
    for item in content:
        print item
        #con = sqlite3.connect("/home/mkoi/mgr_wyniki/database.db")
        #cur = con.cursor()
        #insert_values(cur, 'test3', 'test2')
    #cur.fetchone()

__main__()
