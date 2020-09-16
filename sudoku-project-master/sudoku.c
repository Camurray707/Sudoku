#include <stdio.h>  
#include <unistd.h> 
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

/* These are the only two global variables allowed in your program */
static int verbose = 0;
static int use_fork = 0;

struct Sudoku {
    int array[9][9];
};
// This is a simple function to parse the --fork argument.
// It also supports --verbose, -v
void parse_args(int argc, char *argv[])
{
    int c;
    while (1)
    {
        static struct option long_options[] =
        {
            {"verbose", no_argument,       0, 'v'},
            {"fork",    no_argument,       0, 'f'},
            {0, 0, 0, 0}
        };
        int option_index = 0;
        c = getopt_long (argc, argv, "vf", long_options, &option_index);
        if (c == -1) break;

        switch (c)
        {
            case 'f':
                use_fork = 1;
                break;
            case 'v':
                verbose = 1;
                break;
            default:
                exit(1);
        }
    }
}

void row_Valid(int m, int n, int array[m][n]);
void col_Valid(int m, int n, int array[m][n]);
void subGrid_Valid(int array[9][9]);


int main(int argc, char *argv[])
{
    parse_args(argc, argv);

    if (verbose && use_fork) {
        printf("We are forking child processes as workers.\n");
    } else if (verbose) {
        printf("We are using worker threads.\n");
    }

    printf("Enter a sudoku solution\n");
    int test = 0;
    struct Sudoku mySudoku;

    //scan from a file using < and place solution in struct array
    for(int i = 0; i < 9; i++) {
        for (int j = 0 ; j < 9; j++) {
            scanf("%d",&test);
            mySudoku.array[i][j] = test;
        }
    }
    row_Valid(0, 0, &mySudoku.array);
    col_Valid(0, 0, &mySudoku.array);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ", mySudoku.array[i][j]);
        }
        printf("\n");
    }

    return 0;
}

void row_Valid(int m, int n, int array[m][n]) {
    bool testArray[9] = {0,0,0,0,0,0,0,0,0};

    for (int i = 0; i < 9; i++) {
        int number = array[m][i];
        if(array[m][i] < 1 || array[m][i] > 9 || testArray[(array[m][i])-1] == 1) {
            printf("Row %d doesn't have the required values.\n", m + 1);
            return;
        }
        else {
            testArray[array[m][i]-1] = 1;
        }
    }
}

void col_Valid(int m, int n, int array[m][n]) {
    bool testArray[9] = {0,0,0,0,0,0,0,0,0};

    for (int i = 0; i < 9; i++) {
        int number = array[i][n];
        if (array[i][n] < 1 || array[i][n] > 9 || testArray[(array[i][n])-1] == 1) {
            printf("Column %d doesn't have the required values.\n", n + 1);
            return;
        }
        else {
            testArray[array[i][n]-1] = 1;
        }
    }
}
void subGrid_Valid(int array[9][9]) {

}
