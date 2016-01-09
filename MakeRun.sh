#!/bin/bash
#Efface Ecran
clear
echo "-----COMPILATION DU PROGRAMME-----"
#Suppression de L'Ancien Executable
rm GestionContainers
#Creation Executable
make
make clean
#Droit de ROOT
echo "-----ATTRIBUTION DES DROITS ROOT -----"
chown root GestionContainers
chmod 4550 GestionContainers
echo "-----LANCEMENT-----"
./GestionContainers
