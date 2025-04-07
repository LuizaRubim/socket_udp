# Aplicação Socket UDP

Este repositório contém uma implementação simples de um sistema de comunicação baseado em sockets UDP (User Datagram Protocol). O UDP é um protocolo sem conexão que permite comunicação rápida entre clientes e servidores sem estabelecer uma conexão prévia.

## Visão Geral

Este projeto demonstra a implementação de sockets UDP para comunicação em rede. O UDP é frequentemente utilizado em aplicações onde a velocidade é mais importante que a confiabilidade, como streaming de mídia, jogos online ou aplicações VoIP.

## Pré-requisitos

Para executar este projeto UDP em C, você precisará:

- Compilador C (como GCC ou Clang)
- Sistema operacional compatível (Linux/Unix recomendado, Windows com WSL, ou macOS)
- Make (para usar o arquivo Makefile)

## Instalação

Clone o repositório para sua máquina local:

```bash
git clone https://github.com/LuizaRubim/socket_udp.git
cd socket_udp
```

## Executando a Aplicação

A aplicação consiste em componentes de servidor e cliente que podem ser executados facilmente usando o Makefile fornecido.

### Usando o Makefile

Primeiramente, digite no arquivo client.txt o que você deseja enviar.

Para executar a aplicação, você precisará abrir dois terminais:

**Terminal 1 (Servidor):**
Compilar a aplicação:
```bash
make
```

Executar o receiver:
```bash
sudo ./ receiver
```


**Terminal 2 (Cliente):**
Executar o sender:
```bash
sudo ./ sender
```

Cada comando iniciará o componente correspondente da aplicação.

## Estrutura do Projeto

```
socket_udp/
│
├── sender          # Executável do cliente UDP (compilado de socket_client_UDP.c)
└── receiver        # Executável do servidor UDP (compilado de socket_server_UDP.c)
├── socket_server_UDP.c      # Implementação do servidor UDP em C
├── socket_client_UDP.c     # Implementação do cliente UDP
├── Makefile         # Arquivo para automatizar os passos de compilação e execução
└── README.md        # Instruções para configuração, execução e solução de problemas
```

## Notas sobre UDP

Diferente do TCP, o UDP:
- Não é orientado a conexão
- Não garante a entrega
- Não preserva a ordem das mensagens
- Possui menor latência
- Tem implementação mais simples

## Solução de Problemas

Se você encontrar problemas com as portas, tente:
1. Utilizar um número de porta diferente
2. Verificar se não há outra aplicação usando a mesma porta
3. Executar a aplicação com as permissões adequadas
