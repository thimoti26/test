#include "Define.h"

SOCKET sock;

string GetMacAddr() {
    FILE* Fichier = fopen("/sys/class/net/eth0/address", "r");
    char Adresse[40];
    fgets(Adresse, sizeof (Adresse), Fichier);
    fclose(Fichier);
    return Adresse;
}

void init_connection(const char *address) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN sin = {0};
    struct hostent *hostinfo;

    if (sock == INVALID_SOCKET) {
        perror("socket()");
        exit(errno);
    }

    hostinfo = gethostbyname(address);
    if (hostinfo == NULL) {
        fprintf(stderr, "Unknown host %s.\n", address);
        exit(EXIT_FAILURE);
    }

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
