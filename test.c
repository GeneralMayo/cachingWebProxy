#include <stdio.h>


int main()
{
    int i, j, k;
    for(i=0; i<31; i+=8)
    {
        for(j=0; j<32; j++)
        {
            for(k=0; k<8; k++)
            {

                printf("i=%d, j=%d, k=%d\n rowA=%d, colA=%d\n rowB=%d, colB=%d\n",i,j,k,j,i+k,i+k,j);
                //if(i+k!=j)
                //{
                //tmp=A[j][i+k];
                //B[i+k][j]=tmp;
                //}
            }
        }
    }

    return 0;
}