#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 4
time_t t;

void print_minefield(char **minefield);

void start_minefield(char **minefield);

void initialize_int_2d(int **intminefield);

void initialize_2d(char **minefield);

bool between(int min, char number, int max);

void complete_minefield(char **minefield, int value1, int value2);

void file_matrix(int **intmatrix, char *filedir);

void randomize_bombs(char **minefield);