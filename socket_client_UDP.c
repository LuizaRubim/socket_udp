#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define BUF_SIZE 1024
#define CLIENT_PORT 8888

struct pseudo_header {
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t udp_length;
};

unsigned short checksum(unsigned short *ptr, int nwords) {
    unsigned long sum = 0;
    for (; nwords > 0; nwords--) {
        sum += *ptr++;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

int main() {
    FILE *fp = fopen("client.txt", "r");
    if (!fp) {
        perror("Erro ao abrir client.txt");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sock < 0) {
        perror("Erro ao criar raw socket");
        return 1;
    }

    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(SERVER_PORT);
    dest.sin_addr.s_addr = inet_addr("127.0.0.1");

    char buffer[BUF_SIZE];
    char payload[BUF_SIZE - sizeof(struct udphdr)];

    while (fgets(payload, sizeof(payload), fp)) {
        memset(buffer, 0, BUF_SIZE);

        // Monta cabeçalho UDP
        struct udphdr *udp = (struct udphdr *)buffer;
        udp->source = htons(CLIENT_PORT);
        udp->dest = htons(SERVER_PORT);
        udp->len = htons(sizeof(struct udphdr) + strlen(payload));
        udp->check = 0;

        // Copia payload
        memcpy(buffer + sizeof(struct udphdr), payload, strlen(payload));

        // Checksum UDP com pseudo-header
        struct pseudo_header psh;
        psh.source_address = inet_addr("127.0.0.1");
        psh.dest_address = dest.sin_addr.s_addr;
        psh.placeholder = 0;
        psh.protocol = IPPROTO_UDP;
        psh.udp_length = htons(sizeof(struct udphdr) + strlen(payload));

        int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(payload);
        char *pseudogram = malloc(psize);
        memcpy(pseudogram, &psh, sizeof(struct pseudo_header));
        memcpy(pseudogram + sizeof(struct pseudo_header), udp, sizeof(struct udphdr) + strlen(payload));

        udp->check = checksum((unsigned short *) pseudogram, psize / 2);
        free(pseudogram);

        // Envia
        sendto(sock, buffer, sizeof(struct udphdr) + strlen(payload), 0, (struct sockaddr *)&dest, sizeof(dest));
        printf("[ENVIADO] %s", payload);
        usleep(10000);
    }

    // Envia pacote final com "END"
    char end_msg[] = "END";
    memset(buffer, 0, BUF_SIZE);
    struct udphdr *udp = (struct udphdr *)buffer;
    udp->source = htons(12345);
    udp->dest = htons(SERVER_PORT);
    udp->len = htons(sizeof(struct udphdr) + strlen(end_msg));
    udp->check = 0;

    struct pseudo_header psh;
    psh.source_address = inet_addr("127.0.0.1");
    psh.dest_address = dest.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_UDP;
    psh.udp_length = htons(sizeof(struct udphdr) + strlen(end_msg));

    int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(end_msg);
    char *pseudogram = malloc(psize);
    memcpy(pseudogram, &psh, sizeof(struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header), udp, sizeof(struct udphdr));
    memcpy(buffer + sizeof(struct udphdr), end_msg, strlen(end_msg));
    udp->check = checksum((unsigned short *) pseudogram, psize / 2);
    free(pseudogram);

    sendto(sock, buffer, sizeof(struct udphdr) + strlen(end_msg), 0, (struct sockaddr *)&dest, sizeof(dest));
    printf("[ENVIADO] END\n");

    fclose(fp);
    close(sock);
    return 0;
}
