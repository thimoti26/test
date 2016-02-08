#!/usr/bin/env python
import Docker
import Client
from io import BytesIO
import time
# -*- coding: utf-8 -*-
import sys
reload(sys) # ugly hack to access sys.setdefaultencoding
sys.setdefaultencoding('utf-8')
sys.setrecursionlimit(9999)



print 'Attente de connection'
GestionDocker = Docker.GestionDocker()
LeSocket = Client.SocketClient('192.168.1.189',9999,1)
LeSocket.Connection()
GestionDocker.SetSocket(LeSocket)
print 'Connecte'
ReponseCommande = ''
while(ReponseCommande != 'end'):
    MessageRecu = LeSocket.Reception()
    print 'Recu ' + str(MessageRecu)
    ReponseCommande = GestionDocker.Execute(MessageRecu)
    LeSocket.Envoi(ReponseCommande)
print 'Fin'
#container = cli.create_container (image='thimoti26/test',command = 'zerobin --settings-file=/home/settings.py', ports = [7777,8000])
#print cli.containers()