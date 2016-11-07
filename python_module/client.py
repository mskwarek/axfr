import socket
import sys
import time

host = '127.0.0.1'
port = 50000
size = 1024
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
sys.stdout.write('%')

line = str(time.ctime(time.time())) + '\n'
s.send(line)
data = s.recv(size)
sys.stdout.write(data)
s.close()
