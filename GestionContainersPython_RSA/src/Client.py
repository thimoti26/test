import socket
import time
import base64
import json
import sys
import chilkat
import netifaces as nif
from os import path
from threading import Timer

class SocketClient: 
    PubKey = None
    BUFFER_SIZE = 1024

    def __init__(self, Ip, Port, TempsTimeOut):
        self.Ip = Ip
        self.Port = Port
        self.TempsTimeOut = TempsTimeOut

    def SetTimeOut(self):
        self.Sock.settimeout(self.TempsTimeOut)

    rsa = chilkat.CkRsa()
    RsaRecep = None
    privateKey = chilkat.CkPublicKey()
    
    def Connection(self):
        try:
            
    
            self.rsa = chilkat.CkRsa()
            self.RsaRecep = None
            self.privateKey = chilkat.CkPublicKey()
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
    
    def timeout(self):
        print 'TIMEOUT'
        self.ContinueRecept = False
    
    def SetPubKey(self, key):
        #################STOCK PUB KEY##########################################
        cle = chilkat.CkPublicKey()
        success = self.rsa.UnlockComponent("Anything for 30-day trial")
        if (success != True):
            print(self.rsa.lastErrorText())
            sys.exit()
        self.rsa.put_EncodingMode("hex")
        print 'Cle : ' + str(key)
        success = cle.LoadOpenSslPem(str(key))
        if (success != True):
            print(cle.lastErrorText())
            sys.exit()
        success = self.rsa.ImportPublicKeyObj(cle)
        if (success != True):
            print(self.rsa.lastErrorText())
            sys.exit()
            
        ###################GENERE KEYS##########################################
        self.RsaRecep = chilkat.CkRsa()
        success = self.RsaRecep.UnlockComponent("Anything for 30-day trial")
        self.RsaRecep.put_EncodingMode("hex")
        if (success != True):
            print(self.RsaRecep.lastErrorText())
            sys.exit()
        success = self.RsaRecep.GenerateKey(1024)
        if (success != True):
            print(self.RsaRecep.lastErrorText())
            sys.exit()

        publicKey = self.RsaRecep.ExportPublicKeyObj().getOpenSslPem()
        
        ExecCode = json.dumps({'pubkey' : str(publicKey)})
        self.Envoi(ExecCode)    
    
    def CrypteMsg(self, msg):
        print 'Send : '
        print 'Decrypte : ' + msg 
        mess = self.rsa.encryptStringENC(str(msg),False)
        print 'Crypte : ' + mess 
        return mess  
    
    def DecrypteMsg(self, msg):
        try:
            print 'Recv : '
            print 'Crypte : ' + msg 
            mess = self.RsaRecep.decryptStringENC(str(msg),True)
            print 'Decrypte : ' + mess 
        except Exception as e:
            print "erreur decryptage : " + str(self.RsaRecep.lastErrorText())
        return mess
        
    def Reception(self):
        try:
            self.ContinueRecept = True
            commande = ''
            caractere = ''
            print 'debut'
            while self.ContinueRecept == True:
                caractere = self.Sock.recv(1)
                if(caractere == ''):
                    self.Connection()
                else:
                    message = ''
                    commande = commande + caractere
                    try:
                        message = commande
                        if self.RsaRecep != None:
                            message = self.DecrypteMsg(message)
                        json.loads(message)
                        return message
                    except Exception as e:
                        print 'E : ' + str(e)
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
                sent = self.Sock.send(str(self.CrypteMsg(Message)))
                if sent == 0:
                    print 'Probleme'
                    self.Connection()
            except socket.error as msg:
                print 'Probleme'
                self.Connection()
        
