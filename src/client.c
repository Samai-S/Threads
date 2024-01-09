#include "common.h"
#include "campo_minado.h"
#include "actions.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSZ 1024
#define SIZE 4

void usage(int argc, char **argv)
{
    printf("usage: %s <server IP> <server port>\n", argv[0]);
    printf("example: %s 127.0.0.1 8080\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    struct action act;
    struct action act_answer;
    int start_game = 0;
    int exit_game = 0;
    initialize_struct(&act);
    char string[11];
    char **minefield = malloc(SIZE * sizeof(char *));
    int **matrix = malloc(SIZE * sizeof(int *));
    initialize_int_2d(matrix);

    if (argc < 3)
        usage(argc, argv);

    struct sockaddr_storage storage;
    if (0 != addrparse(argv[1], argv[2], &storage))
        usage(argc, argv);

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1)
        logexit("socket");

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (connect(s, addr, sizeof(storage)) != 0)
        logexit("connect");

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);
    printf("connected to %s\n", addrstr);
    receive_action(s, &act_answer);

    while (1)
    {
        do
        {
            act.cordinates[0]=-1;
            act.cordinates[1]=-1;
            char buf[BUFSZ];
            memset(buf, 0, BUFSZ);
            if (start_game == 0)
                printf("Type 'start' to begin (or type 'exit' to quit): ");
            else
                printf("Enter a message (or type 'exit' to quit): ");

            fgets(buf, BUFSZ - 1, stdin);
            sscanf(buf, "%12s %d,%d", string, &act.cordinates[0], &act.cordinates[1]);
            if (strcmp(buf, "start\n") == 0)
            {
                start_game = 1;
            }

            if (strcmp(buf, "exit\n") == 0)
            {
                exit_game = 1;
                break;
            }

            action_type(&act, string);
        } while (!check_errors(&act) || start_game == 0);
        if (exit_game == 1)
            break;

        do
        {
            if (act.type == 0) 
            {
                act.type = 0;
                send_action(s, &act);
                initialize_2d(minefield);
                print_struct(&act);
            }
            else if (act.type == 1) 
            {
                send_action(s, &act);
                receive_action(s, &act);
            }
            else if (act.type == 2) 
            {
                send_action(s, &act);
                receive_action(s, &act);
            }
            else if (act.type == 3) 
            {
                act.type = 0;
                print_struct(&act);
            }
            else if (act.type == 4)
            {
                send_action(s, &act);
                receive_action(s, &act);
            }
            else if (act.type == 5) 
            {
                send_action(s, &act);
                receive_action(s, &act);
            }
            else if (act.type == 6) 
            {
                send_action(s, &act);
                receive_action(s, &act);
                printf("YOU WIN!\n");
                print_struct(&act);
                act.type = -1;
                close(s);
                break;
            }
            else if (act.type == 7) 
            {

                send_action(s, &act);
                receive_action(s, &act);
            }
            else if (act.type == 8) 
            {
                printf("GAME OVER\n");
                print_struct(&act_answer);
                act.type = -1;
                close(s);
                break;
            }
            else
                printf("\n\n loop \n\n");

        } while (act.type == 3 || act.type == 6 || act.type == 8);
        if (act.type == -1)
            break;
    }

    close(s);
    printf("Connection closed.\n");

    exit(EXIT_SUCCESS);
}
