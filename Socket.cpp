#include "Define.h"

SOCKET sock;
bool Connecte = false;

string GetMacAddr() {
    FILE* Fichier = fopen("/sys/class/net/eth0/address", "r");
    char Adresse[40];
    fgets(Adresse, sizeof (Adresse), Fichier);
    fclose(Fichier);
    return Adresse;
}

void init_connection() {
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET) {
        perror("socket()");
        exit(errno);
    }
}

void SetUnblocked() {
    int ret = 0;
    fd_set write_fds;
    FD_ZERO(&write_fds); //Zero out the file descriptor set
    FD_SET(sock, &write_fds); //Set the current socket file descriptor into the set
    //We are going to use select to wait for the socket to connect
    struct timeval tv; //Time value struct declaration
    tv.tv_sec = 0; //The second portion of the struct
    tv.tv_usec = 1; //The microsecond portion of the struct

    //DEBUG: This is ALWAYS 1
    int select_ret = select(sock + 1, NULL, &write_fds, NULL, &tv);
    if(select_ret < 0){
      perror("select()");
      exit(errno);
    }
   if(ret == 0)
   {
      cout<<"coucou"<<endl;//ici le code si la temporisation (dernier argument) est écoulée (il faut bien évidemment avoir mis quelque chose en dernier argument).
   }
   if(ret == 1)
   {
      cout<<"salut"<<endl;//ici le code si la temporisation (dernier argument) est écoulée (il faut bien évidemment avoir mis quelque chose en dernier argument).
   }
}

void Connection(const char* address) {
    struct hostent *hostinfo;

    hostinfo = gethostbyname(address);
    if (hostinfo == NULL) {
        fprintf(stderr, "Unknown host %s.\n", address);
        exit(EXIT_FAILURE);
    }
    SOCKADDR_IN sin = {0};
    sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    if (connect(sock, (SOCKADDR *) & sin, sizeof (SOCKADDR)) == SOCKET_ERROR) {
        perror("connect()");
        exit(errno);
    }
}

void end_connection() {
    closesocket(sock);
}

string read_server() {
    int n = 0;
    char buffer[BUF_SIZE];
    if ((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0) {
        perror("recv()");
        exit(errno);
    }
    buffer[n] = 0;

    return buffer;
}

void write_server(const char *buffer) {
    if (send(sock, buffer, strlen(buffer), 0) < 0) {
        perror("send()");
        exit(errno);
    }
}
