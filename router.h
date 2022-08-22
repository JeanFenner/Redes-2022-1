#ifndef ROUTER_H_
#define ROUTER_H_

#define BUFLEN 512                              // Tamanho maximo do buffer do pacote
#define MSGLEN 470                              // Tamanho maximo da mensagem do pacote

typedef struct distance_vector {
    int id;
    int cost;
    struct distance_vector *next;
}dist_vec;                                      // Estrutura do Vetor-Distancia

typedef struct router_table {
    int id;
    char ip[15];
    int port;
    int cost;

    struct router_table *next;
}rout_tab;                                      // Estrutua da Tabela de Roteamento

typedef struct configurations {
    char ip[15];
    int id;
    int port;
    rout_tab *r_t;
    dist_vec *d_v;
}configs;                                       // Estrutura de armazendamento de info dos arq. de configuraçao

typedef struct packets {                         // Estrutura do pacote transmitido e recebido
    int type;               // 0-CTRL 1-DATA
    char o_ip[15];          // Origem IP
    int o_port;             // Origem PORT
    char d_ip[15];          // Destino IP
    int d_port;             // Destino PORT
    char message[MSGLEN];   // Dados
}packets;

dist_vec *create_d_v(dist_vec *d_v, int id, int cost);

rout_tab *create_r_t(rout_tab *r_t, int id, char ip[15], int cost);

packets *create_packet(int type, char o_ip[15], int o_port, char d_ip[15], int d_port, char message[MSGLEN]);

configs *init_cfg();

int get_cost(int id, dist_vec *d_v);

int get_dest_port(int dest, rout_tab *r_t);

char *get_dest_ip(int dest, rout_tab *r_t);

configs *config(configs *cfg);

void die(char *s);

void sender(char message[MSGLEN], int DEST_PORT, char DEST_IP[]);

void receiver();

void terminal();

#endif