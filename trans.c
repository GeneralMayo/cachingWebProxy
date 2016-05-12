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
#include "contracts.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. The REQUIRES and ENSURES from 15-122 are included
 *     for your convenience. They can be removed if you like.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    REQUIRES(M > 0);
    REQUIRES(N > 0);

    int i, j, k,l;

    int diagonalValue;

    //flag that indicates a diagonal was stored
    int newdiag=0;

    if(M==32)
    {
        //iterates horizontally through colombs of 8 for A
        for(i=0; i<32; i+=8)
        {
            //iterates vertically down all rows of A
            for(j=0; j<32; j++)
            {
                //iterates horizontally through block of 8 for A
                for(k=0; k<8; k++)
                {
                    if(i+k!=j)
                    {
                        B[i+k][j]=A[j][i+k];
                    }
                    else
                    {
                        diagonalValue=A[j][i+k];
                        newdiag=1;
                    }
                    if(k==7 && newdiag==1)
                    {
                        B[j][j]=diagonalValue;
                        newdiag=0;
                    }
                }
            }
        }
    }
    else if(M==64)
    {
        //iterating horizontally over "4 blocks" in A
        //iterating vertically over "4 blocks" in B
        for(i=0; i<64; i+=4)
        {
            //iterating vertically over "4 blocks" in A
            //iterating horizontally over "4 blocks" in B
            for(j=0; j<64; j+=4)
            {
                //iterating over rows of A "4 block"
                //iterating over cols of B "4 block"
                for(k=0; k<4; k++)
                {
                    //iterating over cols of A "4 block"
                    //iterating over rows of B "4 block"
                    for(l=0; l<4; l++)
                    {
                        //in a diagonal "8 block" and on 
                        //diagonal in "4 block
                        if(((i-j<=4)||(j-i<=4)) && l==k)
                        {
                            diagonalValue=A[j+k][i+l];
                            newdiag=1;
                        }
                        else
                        {
                            B[i+l][j+k]=A[j+k][i+l];
                        }
                        if(l==3 && newdiag)
                        {
                            B[i+k][j+k]=diagonalValue;
                            newdiag=0;
                        }
                    }
                }
            }   
        }
    }
    else if(M==61)
    {
        //using blocks of 23
        //iterating horizontally over "23 blocks" in A
        //iterating vertically over "4 blocks" in B
        for(i=0; i<67; i+=23)
        {
            //iterating vertically over "23 blocks" in A
            //iterating horizontally over "23 blocks" in B
            for(j=0; j<61; j+=23)
            {
                //iterating over rows of A "23 block"
                //iterating over cols of B "23 block"
                for(k=0; k<23; k++)
                {
                    //iterating over cols of A "23 block"
                    //iterating over rows of B "23 block"
                    for(l=0; l<23; l++)
                    {
                        if(i+k<67 && j+l<61)
                        {
                            B[j+l][i+k]=A[i+k][j+l];
                        }
                    }
                }
            }   
        }
    }

    ENSURES(is_transpose(M, N, A, B));
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */


//char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    REQUIRES(M > 0);
    REQUIRES(N > 0);


    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

    ENSURES(is_transpose(M, N, A, B));
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
    //registerTransFunction(trans, trans_desc); 

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

