#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 4

void initialize_int_2d(int **intminefield)
{
    for (int i = 0; i < SIZE; i++)
    {
        intminefield[i] = (int *)malloc(SIZE * sizeof(int));
    }
}

void initialize_2d(char **minefield)
{
    for (int i = 0; i < SIZE; i++)
    {
        minefield[i] = (char *)malloc(SIZE * sizeof(char));
    }
}

bool between(int min, char number, int max)
{

    return (number > min && number < max);
}

void file_matrix(int **intmatrix, char *filedir)
{
    FILE *ptr;
    ptr = fopen(filedir, "r");
    char c;
    int i = 0, j = 0, negative = 1;

    while (fscanf(ptr, "%c", &c) != EOF)
    {
        if (c == '-')
            negative = -1;

        if (c != ' ' && c != ',' && c != '\n' && c != '-')
        {
            if (j >= 0 && j < 4)
            {
                intmatrix[i][j] = (c - 48) * negative;
                negative = 1;
            }
            j++;
            if (j == 4)
            {
                j = 0;
                i++;
                if (i == 4)
                    break;
            }
        }
    }
}

void inttocharmatrix(char **minefield, int minefield_int[][4])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            
            if(minefield_int[i][j] == -2)
            minefield[i][j] = '-';
            else if(minefield_int[i][j]==-1)
            minefield[i][j]='*';
            else if(minefield_int[i][j]==0)
            minefield[i][j]= '0';
            else if(minefield_int[i][j]==-3)
            minefield[i][j]='>';
            else 
            minefield[i][j]=minefield_int[i][j]+48;

        }
    }

}
