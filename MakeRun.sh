#!/bin/bash
#Creation Executable
make
make clean
#Droit de ROOT
chown root GestionContainers
chmod 4550 GestionContainers
./GestionContainers
