#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int main() {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sock < 0) {
        perror("Erro ao criar raw socket");
        return 1;
    }

    char buffer[BUF_SIZE];

    printf("[ESPERANDO] Pacotes UDP via loopback...\n");

    while (1) {
        memset(buffer, 0, BUF_SIZE);

        int len = recvfrom(sock, buffer, BUF_SIZE, 0, NULL, NULL);
        if (len < 0) {
            perror("Erro no recvfrom");
            continue;
        }

        struct iphdr *ip = (struct iphdr *) buffer;
        int iphdrlen = ip->ihl * 4;
        struct udphdr *udp = (struct udphdr *) (buffer + iphdrlen);
        char *data = buffer + iphdrlen + sizeof(struct udphdr);

        int data_len = ntohs(udp->len) - sizeof(struct udphdr);
        char msg[data_len + 1];
        memcpy(msg, data, data_len);
        msg[data_len] = '\0';

        printf("[RECEBIDO] %s\n", msg);

        if (strcmp(msg, "END") == 0) {
            printf("[FIM] Mensagem END recebida.\n");
            break;
        }
    }

    close(sock);
    return 0;
}
