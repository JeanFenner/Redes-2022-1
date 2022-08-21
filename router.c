/*
Universidade Federal da Fronteira Sul
Campus Chapecó - SC
Curso de Ciência da Computação
Redes de Computadores – 2022.1
Profº Marco Aurélio Spohn

Programação Socket e Roteamento

Simulação de protocolo de roteamento entre processos roteadores com sockets UDP.

    * O código presente nas funções client() e server() foram retiradas do material didático do Professor Marco Aurélio Spohn e alterados.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "router.h"

configs *configuration;

int main() {
    pthread_t t_listen;
    pthread_t t_terminal;

    int id, err;

    configuration = init_cfg();

    printf("Starting...\n\n");
    printf("ID: ");
    scanf("%d", &configuration->id);

    configuration = config(configuration);

    err = pthread_create(&t_listen, NULL, receiver, NULL);
    if(err) {
        printf("Receiver Error!");
        exit(-1);
    }
    
    err = pthread_create(&t_terminal, NULL, terminal, NULL);
    if(err) {
        printf("Receiver Error!");
        exit(-1);
    }

    pthread_join(t_terminal, 0);
    pthread_cancel(t_listen);
    pthread_exit(NULL);

    return 0;
}

dist_vec *create_d_v(dist_vec *d_v, int id, int cost) {                          // Criaçao de objetos Vetor-Distancia
    dist_vec *new = (dist_vec *)malloc(sizeof(dist_vec));

    new->id = id;
    new->cost = cost;
    new->next = d_v;

    return new;
}

rout_tab *create_r_t(rout_tab *r_t, int id, char ip[15], int port) {   // Criaçao de objetos Tabela de Roteamento
    rout_tab *new = (rout_tab *)malloc(sizeof(rout_tab));

    new->id = id;
    strcpy(new->ip, ip);
    new->port = port;
    new->next = r_t;

    return new;
}

configs *init_cfg() {                           // Inicializaçao de estrutura
    configs *cfg = (configs *)malloc(sizeof(configs));

    cfg->r_t = NULL;
    cfg->d_v = NULL;
    
    return cfg;
}

int get_cost(int id, dist_vec *d_v) {           // Obter Custo de Vetor-Distancia
    while(d_v) {
        if(d_v->id == id)
            return d_v->cost;
        d_v = d_v->next;
    }

    return -1;
}

int get_dest_port(int dest, rout_tab *r_t) {    // Obter Porta da Tabela de Roteamento
    rout_tab *temp = r_t;

    while(temp) {
        if(dest == temp->id)
            return temp->port;
        
        temp = temp->next;
    }

    return -1;
}

char *get_dest_ip(int dest, rout_tab *r_t) {    // Obter IP da Tabela de Roteamento
    rout_tab *temp = r_t;

    while(temp) {
        if(dest == temp->id)
            return temp->ip;
        
        temp = temp->next;
    }

    return "0";
}

configs *config(configs *cfg) {
    FILE* links = fopen("links.config", "r");
    FILE* routers = fopen("routers.config", "r");
    int id_o, id_d, cost, x, port, ip_i[4];
    char c, *temp, ip_c[15], dump;

    if(links) {
        while(c!=(EOF)) {
            fscanf(links, "%d %d %d", &id_o, &id_d, &cost);
            if(id_o == cfg->id) {
                cfg->d_v = create_d_v(cfg->d_v, id_d, cost);
            } else if(id_d == cfg->id) {
                cfg->d_v = create_d_v(cfg->d_v, id_o, cost);;
            }
            c = fgetc(links);
        }    
    } else{
        printf("Arquivo 'links.config' nao pode ser aberto!");
    }
    fclose(links);

    c = ' ';

    if(routers) {
        while(c!=EOF) {
            fscanf(routers, "%d %d %d %c %d %c %d %c %d", &id_d, &port, &ip_i[0], &dump, &ip_i[1], &dump, &ip_i[2], &dump, &ip_i[3]);
            sprintf(&ip_c, "%d.%d.%d.%d",ip_i[0], ip_i[1], ip_i[2], ip_i[3]);
            if(id_d == cfg->id) {
                strcpy(&cfg->ip, ip_c);
                cfg->port = port;
            }
            cost = get_cost(id_d, cfg->d_v);
            if(cost>0) {
                cfg->r_t = create_r_t(cfg->r_t, id_d, ip_c, port);
            }
            c = fgetc(routers);
        }
    } else{
        printf("Arquivo 'routers.config' nao pode ser aberto!");
    }

    fclose(routers);

    return cfg;
}

void die(char *s) {
    perror(s);
    exit(1);
}

void sender(char message[BUFLEN], int DEST_PORT, char DEST_IP[]) {
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
 
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(DEST_PORT);

    if (inet_aton(DEST_IP , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
 
    //send the message
    if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
    {
        die("sendto()");
    }
    //clear the buffer by filling null
    memset(buf,'\0', BUFLEN);
    
    close(s);
}

void receiver() {
    struct sockaddr_in si_me, si_other;
     
    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];
     
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(configuration->port);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
    //keep listening for data
        fflush(stdout);
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);

        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
         
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);

    close(s);
}

void terminal() {
    int cont=1, opc, dest, port;
    char msg[BUFLEN], ip[15];
    rout_tab *temp=NULL;

    while(cont) {

        printf("\n\tMENU\nROUTER %d: %s\n", configuration->id, configuration->ip);
        printf("[1] - Enviar mensagem\n");
        printf("[2] - Listar destinos\n");
        printf("[0] - Encerrar\n");
        scanf("%d", &opc);

        switch(opc) {
            case(1):
                printf("Destino: ");
                scanf("%d", &dest);
                printf("Mensagem: ");
                scanf("%s", msg);
                sender(msg, get_dest_port(dest, configuration->r_t), get_dest_ip(dest, configuration->r_t));
                break;
            case(2):
                printf("Destinos\n");
                temp = configuration->r_t;
                while(temp) {
                    printf("ID: %d\t", temp->id);
                    printf("IP: %s\t", temp->ip);
                    printf("PORT: %d\n", temp->port);
                    temp = temp->next;
                }
                break;
            case(0):
                cont = 0;
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    }

    printf("Encerrando...\n");
}