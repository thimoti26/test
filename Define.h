#ifndef DEFINE_H
#define	DEFINE_H
#include <signal.h>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <vector>
#include <fstream>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
#define CLOCKID CLOCK_REALTIME
#define SIG SIGRTMIN
#define PORT 9999
#define BUF_SIZE 1024

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

using namespace std;

typedef struct dictionnaire {
    string Container;
    string Repository;
    string ContainerIDAssocie;
    string ImageIDAssocie;
    bool IsRunning;
} dictionnaire;


////////////////Prototype/////////////////
/////////SOCKET//////////
string GetMacAddr();
void init_connection();
void SetUnblocked();
void Connection(const char *);
void end_connection();
string read_server();
void write_server(const char*);
/////////DOCKER//////////
void Login(string, string, string);
string GetMessageFromCommand(string, bool TakeCarriageReturn = true);
void Pull(string);
void DeleteImage(string);
string GetImageId(string, bool);
string GetContainerIdFromImage(string, bool = false);
string GetImageIdFromContainer(string, bool = false);
void StopContainerId(string);
void DeleteContainerId(string);
void DeleteAllContainers();
void ShowContainers(bool = false);
void ShowImages();
void StopAllContainers();
void DeleteAllImages();
void RunImageId(string, bool = false, bool = false, bool = false, bool = false, string = "", string = "", string = "", bool = false);
/////////TIMER//////////
void Coucou(int);
void handler(int, siginfo_t *, void *);
void InitTimer();
void DemarrageTimer();
/////////FICHIER/////////
FILE* OpenPipe(FILE*, const char*, const char*);
char* ReadPipe(char*, FILE*, int);
void ClosePipe(FILE*);
void OpenFile(fstream&, const char*, std::ios::openmode);
void ReadFile(fstream&, dictionnaire*, int);
void WriteFile(fstream&, dictionnaire*, int);
void Close(fstream&);
#endif	/* DEFINE_H */

