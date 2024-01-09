#include <stdbool.h>

#define SIZE 4

struct action
{
    int type;
    int cordinates[2];
    int board[4][4];
};

void print_struct(struct action *act);
bool check_reveal_errors(struct action *act);
bool check_flag_errors(struct action *act);
bool check_errors(struct action *act);
void send_action(int sock, struct action *act);
void receive_action(int sock, struct action *act);
void action_type(struct action *act, char *action);
void check_win(struct action *act_received);
int reveal_matrix(struct action *act, struct action *act_received);
void flag_matrix(struct action *act_received);
void remove_flag_matrix(struct action *act_received);
void initialize_struct(struct action *act);
void copy_matrix(struct action *act, int **matrix);

