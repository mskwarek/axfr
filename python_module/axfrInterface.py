import select 
import socket 
import sys 
import threading 
import logger

class Server: 
    def __init__(self, sending_data_cb): 
        self.host = '' 
        self.port = 55555
        self.backlog = 5 
        self.size = 1024 
        self.server = None 
        self.threads = []
        self.sending_data_cb = sending_data_cb
 
    def open_socket(self): 
        try: 
            self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
            self.server.bind((self.host,self.port)) 
            self.server.listen(5) 
        except socket.error, (value,message): 
            if self.server: 
                self.server.close() 
            print "Could not open socket: " + message 
            sys.exit(1) 
 
    def run(self): 
        self.open_socket() 
        input = [self.server,sys.stdin] 
        running = 1 
        while running: 
            inputready,outputready,exceptready = select.select(input,[],[]) 
 
            for s in inputready: 
 
                if s == self.server: 
                    # handle the server socket 
                    c = Client(self.server.accept(), self.sending_data_cb) 
                    c.start() 
                    self.threads.append(c) 
 
                elif s == sys.stdin: 
                    # handle standard input 
                    junk = sys.stdin.readline() 
                    running = 0 
 
        # close all threads 
 
        self.server.close() 
        for c in self.threads: 
            c.join() 
 
class Client(threading.Thread): 
    def __init__(self,(client,address), sending_data_cb): 
        threading.Thread.__init__(self) 
        self.client = client 
        self.address = address 
        self.size = 1024
        self.sending_data_cb = sending_data_cb
 
    def run(self): 
        running = 1 
        while running: 
            data = self.client.recv(self.size) 
            if data:
                self.client.send(str(self.sending_data_cb())) 
            else: 
                self.client.close() 
                running = 0 

def dumb_data():
    return 1
                
if __name__ == "__main__": 
    x = logger.Logger()
    s = Server(x.get_iter) 
    s.run()
