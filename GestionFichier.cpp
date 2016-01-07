#include "Define.h"

FILE* OpenPipe(FILE* Fichier, const char* Chemin, const char* Mode) {
    return popen(Chemin, Mode);
}

char* ReadPipe(char* path, FILE* fp, int Size) {
    return fgets(path, Size - 1, fp);
}

void ClosePipe(FILE* Fichier) {
    pclose(Fichier);
}

void OpenFile(fstream& Fichier, const char* Chemin, ios::openmode Mode) {
    Fichier.open(Chemin,Mode);
}

void ReadFile(fstream& Fichier, dictionnaire* dico, int size) {
    Fichier.read((char*)dico,size);
}

void WriteFile(fstream& Fichier, dictionnaire* dico, int size) {
    Fichier.write((char*)dico,size);
}

void Close(fstream& Fichier) {
    Fichier.close();
}