import socket
import time
import json
import netifaces as nif

class SocketClient: 
    BUFFER_SIZE = 1024
    def __init__(self, Ip, Port, TempsTimeOut):
        self.Ip = Ip
        self.Port = Port
        self.TempsTimeOut = TempsTimeOut

    def SetTimeOut(self):
        self.Sock.settimeout(self.TempsTimeOut)

    def Connection(self):
        try:
            self.Sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#            self.SetTimeOut()
            result = self.Sock.connect_ex((self.Ip, self.Port))
            if(result == 0):
                return 1
            else:
                self.Connection()
        except socket.error as msg:
            print 'coucou je suis ici mais je sais pas quoi faire'
            self.Sock = None
        except RuntimeError:
            self.Connection()

    def get_mac_addr(self):
        'Returns a list of MACs for interfaces that have given IP, returns None if not found'
        for i in nif.interfaces():
            addrs = nif.ifaddresses(i)
            try:
                if_mac = addrs[nif.AF_LINK][0]['addr']
                if_ip = addrs[nif.AF_INET][0]['addr']
            except IndexError, KeyError: #ignore ifaces that dont have MAC or IP
                if_mac = if_ip = None
            if if_ip == self.Sock.getsockname()[0]:
                time.sleep(3)
                return if_mac
        return None
    

    def Reception(self):
        try:
            commande = ''
            caractere = ''
            while True:
                caractere = self.Sock.recv(1)
                if(caractere == ''):
                    self.Connection()
                else:
                    commande = commande + caractere
                    try:
                        json.loads(commande)
                        print 'c : '+ str(commande)
                        return commande
                    except Exception as e:
                        continue
        except socket.timeout:
            return
        except socket.error as msg:
            print 'Probleme'
            self.Connection()

    def Envoi(self,Message):
        if(Message != None):
            try:
                print 'r : '+str(Message)
                sent = self.Sock.send(str(Message))
                if sent == 0:
                    print 'Probleme'
                    self.Connection()
            except socket.error as msg:
                print 'Probleme'
                self.Connection()
        
