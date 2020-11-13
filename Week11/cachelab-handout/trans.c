//2019054957
/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int blockRow, blockCol;
    int row, col;
    int t1, t2, t3, t4, t5, t6, t7, t8;

    if(M == 32) {
        for(row = 0; row < N; row += 8) {
            for(col = 0; col < M; col += 8) {
                for(blockRow = row; (blockRow < N) && (blockRow < row + 8); blockRow++) {
                    for(blockCol = col; (blockCol < M) && (blockCol < col + 8); blockCol++) {
                        if(blockRow != blockCol) {
                            B[blockCol][blockRow] = A[blockRow][blockCol];
                        }
                        else {
                            t1 = blockRow;
                            t2 = A[blockRow][blockCol];
                        }
                    }
                    if(row == col) {
                        B[t1][t1] = t2;
                    }
                }
            }
        }
    }
    else if(M == 64) {
        for(row = 0; row < N; row += 8) {
            for(col = 0; col < M; col += 8) {
                for(blockRow = 0; blockRow < 4; blockRow++) {
                    t1 = A[row + blockRow][col + 0];
                    t2 = A[row + blockRow][col + 1];
                    t3 = A[row + blockRow][col + 2];
                    t4 = A[row + blockRow][col + 3];
                    t5 = A[row + blockRow][col + 4];
                    t6 = A[row + blockRow][col + 5];
                    t7 = A[row + blockRow][col + 6];
                    t8 = A[row + blockRow][col + 7];
                    B[col + 0][row + blockRow + 0] = t1;
                    B[col + 0][row + blockRow + 4] = t6;
                    B[col + 1][row + blockRow + 0] = t2;
                    B[col + 1][row + blockRow + 4] = t7;
                    B[col + 2][row + blockRow + 0] = t3;
                    B[col + 2][row + blockRow + 4] = t8;
                    B[col + 3][row + blockRow + 0] = t4;
                    B[col + 3][row + blockRow + 4] = t5;
                }

                t1 = A[row + 4][col + 4];
                t2 = A[row + 5][col + 4];
                t3 = A[row + 6][col + 4];
                t4 = A[row + 7][col + 4];
                t5 = A[row + 4][col + 3];
                t6 = A[row + 5][col + 3];
                t7 = A[row + 6][col + 3];
                t8 = A[row + 7][col + 3];

                B[col + 4][row + 0] = B[col + 3][row + 4];
                B[col + 4][row + 4] = t1;
                B[col + 3][row + 4] = t5;
                B[col + 4][row + 1] = B[col + 3][row + 5];
                B[col + 4][row + 5] = t2;
                B[col + 3][row + 5] = t6;
                B[col + 4][row + 2] = B[col + 3][row + 6];
                B[col + 4][row + 6] = t3;
                B[col + 3][row + 6] = t7;
                B[col + 4][row + 3] = B[col + 3][row + 7];
                B[col + 4][row + 7] = t4;
                B[col + 3][row + 7] = t8;
                
                for(blockCol = 0; blockCol < 3; blockCol++) {
                    t1 = A[row + 4][col + 5 + blockCol];
                    t2 = A[row + 5][col + 5 + blockCol];
                    t3 = A[row + 6][col + 5 + blockCol];
                    t4 = A[row + 7][col + 5 + blockCol];
                    t5 = A[row + 4][col + 0 + blockCol];
                    t6 = A[row + 5][col + 0 + blockCol];
                    t7 = A[row + 6][col + 0 + blockCol];
                    t8 = A[row + 7][col + 0 + blockCol];

                    B[col + 5 + blockCol][row + 0] = B[col + blockCol][row + 4];
                    B[col + 5 + blockCol][row + 4] = t1;
                    B[col + 0 + blockCol][row + 4] = t5;
                    B[col + 5 + blockCol][row + 1] = B[col + blockCol][row + 5];
                    B[col + 5 + blockCol][row + 5] = t2;
                    B[col + 0 + blockCol][row + 5] = t6;
                    B[col + 5 + blockCol][row + 2] = B[col + blockCol][row + 6];
                    B[col + 5 + blockCol][row + 6] = t3;
                    B[col + 0 + blockCol][row + 6] = t7;
                    B[col + 5 + blockCol][row + 3] = B[col + blockCol][row + 7];
                    B[col + 5 + blockCol][row + 7] = t4;
                    B[col + 0 + blockCol][row + 7] = t8;
                }
            }
        }
    }
    else {
        for(row = 0; row < N; row += 16) {
            for(col = 0; col < M; col += 16) {
                for(blockRow = row; (blockRow < N) && (blockRow < row + 16); blockRow++) {
                    for(blockCol = col; (blockCol < M) && (blockCol < col + 16); blockCol++) {
                        if(blockRow != blockCol) {
                            B[blockCol][blockRow] = A[blockRow][blockCol];
                        }
                        else {
                            t1 = blockRow;
                            t2 = A[blockRow][blockCol];
                        }
                    }
                    if(row == col) {
                        B[t1][t1] = t2;
                    }
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

