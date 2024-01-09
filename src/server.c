#include "common.h"
#include "campo_minado.h"
#include "actions.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#define BUFSZ 1024
#define SIZE 4

void usage(int argc, char **argv)
{
    printf("usage: %s <v4|v6> <server port>\n ", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    int **intmatrix = malloc(SIZE * sizeof(int *));
    initialize_int_2d(intmatrix);

    if (strcmp(argv[3], "-i") == 0)
    {
        file_matrix(intmatrix, argv[4]);
    }

    struct action act;
    struct action act_received;

    copy_matrix(&act, intmatrix);
    initialize_struct(&act_received);

    if (argc < 3)
        usage(argc, argv);

    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage))
        usage(argc, argv);

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1)
        logexit("socket");

    int enable = 1;
    if (0 != setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)))
        logexit("setsockopt");

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (bind(s, addr, sizeof(storage)) != 0)
        logexit("bind");

    if (listen(s, 10) != 0)
        logexit("listen");

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);
    printf("bound in %s, waiting connection\n", addrstr);

    while (1)
    {
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);

        int csock = accept(s, caddr, &caddrlen);
        if (csock == -1)
            logexit("accept");

        char caddrstr[BUFSZ];
        addrtostr(caddr, caddrstr, BUFSZ);
        printf("[log] connection from %s\n", caddrstr);
        print_struct(&act);
        send_action(csock, &act);

        struct action act_received;
        initialize_struct(&act_received);
        receive_action(csock, &act_received);

        if (act_received.type == 0)
        {
            initialize_struct(&act_received);
            while (1)
            {
                receive_action(csock, &act_received);
                if (act_received.type == 0)
                {
                    print_struct(&act_received);
                }

                else if (act_received.type == 1)
                {
                    act_received.type=3;
                    reveal_matrix(&act, &act_received);
                    send_action(csock, &act_received);

                }
                else if (act_received.type == 2)
                {
                    act_received.type=3;
                    flag_matrix(&act_received);
                    send_action(csock, &act_received);
                }
                else if (act_received.type == 3)
                {
                }
                else if(act_received.type == 4)
                {
                    act_received.type=3;
                    remove_flag_matrix(&act_received);
                    send_action(csock, &act_received);
                }
                else if (act_received.type == 5)
                {
                    act_received.type=3;
                    initialize_struct(&act_received);
                    send_action(csock, &act_received);
                }
                else if (act_received.type == 6)
                {
                    send_action(csock, &act_received);
                }
                else if (act_received.type == 8)
                {
                    act_received.type = 8;

                    send_action(csock, &act);
    
                    break;
                }
                else if (act_received.type == -1)
                {
                    break;
                }
            }
        }      
    }

    close(s);
    exit(EXIT_SUCCESS);
}
