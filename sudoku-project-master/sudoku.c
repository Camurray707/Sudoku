#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h>

/* These are the only two global variables allowed in your program */
static int verbose = 0;
static int use_fork = 0;

struct Sudoku {
    int array[9][9];
    int row_start;
    int col_start;
    int sub_row;
    int sub_col;
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

void *row_Valid(void* arg);
void *col_Valid(void* arg);
void *subGrid_Valid(void* arg);


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
    mySudoku.row_start = 0;
    mySudoku.col_start = 0;
    mySudoku.sub_col = 0;
    mySudoku.sub_row = 0;

    //scan from a file using < and place solution in struct array
    for(int i = 0; i < 9; i++) {
        for (int j = 0 ; j < 9; j++) {
            scanf("%d",&test);
            mySudoku.array[i][j] = test;
        }
    }

    //print formatted sudoku array
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ", mySudoku.array[i][j]);
        }
        printf("\n");
    }

    //threads for row and column checking
    for (int i = 0; i < 9; i++) {
        pthread_t rowThread;
        pthread_create(&rowThread, NULL, row_Valid, &mySudoku);
        pthread_join(rowThread, NULL);

        pthread_t colThread;
        pthread_create(&colThread, NULL, col_Valid, &mySudoku);
        pthread_join(colThread, NULL);
    }

    //threads for subgrid checking
    for (int i = 0; i < 3; i++) {
        pthread_t subGridThread;
        pthread_create(&subGridThread, NULL, subGrid_Valid, &mySudoku);
        pthread_join(subGridThread, NULL);
    }

    return 0;
}


void* row_Valid(void* arg) {
    struct Sudoku *sudoku;
    sudoku = (struct Sudoku *) arg;
    bool testArray[9] = {0,0,0,0,0,0,0,0,0};

    for (int i = 0; i < 9; i++) {
        if(sudoku->array[sudoku->row_start][i] < 1 || sudoku->array[sudoku->row_start][i] > 9 ||
        testArray[(sudoku->array[sudoku->row_start][i])-1] == 1) {
            printf("Row %d doesn't have the required values.\n", sudoku->row_start + 1);
            sudoku->row_start++;
            return NULL;
        }
        else {
            testArray[sudoku->array[sudoku->row_start][i]-1] = 1;
        }
    }
    sudoku->row_start++;
}

void* col_Valid(void* arg) {
    struct Sudoku *sudoku;
    sudoku = (struct Sudoku *) arg;
    bool testArray[9] = {0,0,0,0,0,0,0,0,0};

    for (int i = 0; i < 9; i++) {
        if (sudoku->array[i][sudoku->col_start] < 1 || sudoku->array[i][sudoku->col_start] > 9 ||
        testArray[(sudoku->array[i][sudoku->col_start])-1] == 1) {
            printf("Column %d doesn't have the required values.\n", sudoku->col_start + 1);
            sudoku->col_start++;
            return NULL;
        }
        else {
            testArray[sudoku->array[i][sudoku->col_start]-1] = 1;
        }
    }
    sudoku->col_start++;

}


void* subGrid_Valid(void* arg){
    struct Sudoku *sudoku;
    sudoku = (struct Sudoku *) arg;
    bool testArray[9] = {0,0,0,0,0,0,0,0,0};
    
    for (int r = sudoku->sub_row; r < sudoku->sub_row + 3; r ++) {
        for (int c = sudoku->sub_col; c < sudoku->sub_col + 3; c++) {
            if(sudoku->array[r][c] < 1 || sudoku->array[r][c] > 9 || testArray[(sudoku->array[r][c])-1] == 1){
                if(sudoku->sub_row <= 2)
                    printf("Top ");
                else if(sudoku->sub_row<= 5)
                    printf("Middle ");
                else
                    printf("Bottom ");
    
                if(sudoku->sub_col <= 2)
                    printf("left ");
                else if(sudoku->sub_col <= 5)
                    printf("center ");
                else
                    printf("right ");

                printf("subgrid doesn't have the required values.\n");
                sudoku->sub_row+=3;
                sudoku->sub_col+=3;
                return NULL;
            }
            else {
            testArray[sudoku->array[r][c]-1] = 1;
            }
        }
    }
    sudoku->sub_row+=3;
    sudoku->sub_col+=3;
}






