#include "campo_minado.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include <inttypes.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SIZE 4

struct action
{
    int type;
    int cordinates[2];
    int board[4][4];
};

void print_struct(struct action *act)
{

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if(act->board[i][j]==-2)
                printf("- ");
            else if(act->board[i][j]==-3)
                printf("> ");
            else if(act->board[i][j]==-1)
                printf("* ");
            else 
            printf("%d ", act->board[i][j]);
        }
        printf("\n");
    }
}


bool check_reveal_errors(struct action *act)
{
    if (!between(-1,act->cordinates[0],4)||!between(-1,act->cordinates[1],4))
    {
        printf("error: invalid cell\n");
        print_struct(act);
        return 0;
    }
    if(act->board[act->cordinates[0]][act->cordinates[1]]!=-2&&act->board[act->cordinates[0]][act->cordinates[1]]!=-1&&act->board[act->cordinates[0]][act->cordinates[1]]!=-3)
    {
        printf("error: cell already revealed\n");
        print_struct(act);
        return 0;
    }
    return 1;
    
}

bool check_flag_errors(struct action *act)
{
    if(act->board[act->cordinates[0]][act->cordinates[1]]==-3)
    {
        printf("error: cell already has a flag\n");
        print_struct(act);
        return 0;
    }
    if(act->board[act->cordinates[0]][act->cordinates[1]]!=-2&&act->board[act->cordinates[0]][act->cordinates[1]]!=-1&&act->board[act->cordinates[0]][act->cordinates[1]]!=-3)
    {
        printf("error: cannot insert a flag in revealed cell\n");
        print_struct(act);
        return 0;
    }
    return 1;
}


bool check_errors(struct action *act)
{
    if(act->type == 1)
        return check_reveal_errors(act);
    if(act->type == 2)
        return check_flag_errors(act);
    return 1;
    
}

void send_action(int sock, struct action *act)
{
    if (send(sock, act, sizeof(struct action), 0) == -1)
    {
        printf("start para iniciar o game");
    }
}

void receive_action(int sock, struct action *act)
{
    ssize_t count = recv(sock, act, sizeof(struct action), 0);
    if (count == -1)
    {
        printf(" \n");
    }
    else if (count == 0)
    {
        act->type=-1;
        printf("connection closed\n");
    }
}

void action_type(struct action *act, char *action)
{
    if (strcmp(action, "start\n") == 0 || strcmp(action, "start") == 0)
        act->type = 0;
    else if (strcmp(action, "reveal\n") == 0 || strcmp(action, "reveal") == 0)
        act->type = 1;
    else if (strcmp(action, "flag\n") == 0 || strcmp(action, "flag") == 0)
        act->type = 2;
    else if (strcmp(action, "state\n") == 0 || strcmp(action, "state") == 0)
        act->type = 3;
    else if (strcmp(action, "remove_flag\n") == 0 || strcmp(action, "remove_flag") == 0)
        act->type = 4;
    else if (strcmp(action, "reset\n") == 0 || strcmp(action, "reset") == 0)
        act->type = 5;
    else if (strcmp(action, "win\n") == 0 || strcmp(action, "www") == 0)
        act->type = 6;
    else if (strcmp(action, "exit\n") == 0 || strcmp(action, "exit") == 0)
        act->type = 7;
    else if (strcmp(action, "game_over\n") == 0 || strcmp(action, "ggg") == 0)
        act->type = 8;
    else
        printf("Unknown type of action try again\n");
}

void check_win(struct action *act_recieved)
{
    int count =0;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (act_recieved->board[i][j] != -1 &&act_recieved->board[i][j] != -2 &&act_recieved->board[i][j] != -3 )
                count ++;
        }
    }
    
    if (count==13)
        act_recieved->type = 6;


}

int reveal_matrix(struct action *act, struct action *act_recieved)
{
    printf("revel_matrix runned\n");
    if (act->board[act_recieved->cordinates[0]][act_recieved->cordinates[1]] == -1)
    {
        act_recieved->type = 8;
        return -1;
    }
    else
    {
        act_recieved->board[act_recieved->cordinates[0]][act_recieved->cordinates[1]] = act->board[act_recieved->cordinates[0]][act_recieved->cordinates[1]];
        check_win(act_recieved);
        return 1;
    }
        printf("reveal 0\n");
    return 0;
}

void flag_matrix(struct action *act_recieved)
{
    if(act_recieved->board[act_recieved->cordinates[0]][act_recieved->cordinates[1]] ==-2)
    act_recieved->board[act_recieved->cordinates[0]][act_recieved->cordinates[1]] = -3;
}

void remove_flag_matrix(struct action *act_recieved)
{
    if(act_recieved->board[act_recieved->cordinates[0]][act_recieved->cordinates[1]] ==-3)
    act_recieved->board[act_recieved->cordinates[0]][act_recieved->cordinates[1]] = -2;
}

void initialize_struct(struct action *act)
{

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            act->board[i][j] = -2;
        }
    }
}

void copy_matrix(struct action *act, int **matrix)
{

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            act->board[i][j] = matrix[i][j];
        }
    }
}

