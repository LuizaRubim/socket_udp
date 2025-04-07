# Nome dos executáveis
SENDER = sender
RECEIVER = receiver

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Arquivos fonte
SENDER_SRC = socket_client_UDP.c
RECEIVER_SRC = socket_server_UDP.c

.PHONY: all clean

all: $(SENDER) $(RECEIVER)

$(SENDER): $(SENDER_SRC)
	$(CC) $(CFLAGS) $(SENDER_SRC) -o $(SENDER)

$(RECEIVER): $(RECEIVER_SRC)
	$(CC) $(CFLAGS) $(RECEIVER_SRC) -o $(RECEIVER)

clean:
	rm -f $(SENDER) $(RECEIVER)
