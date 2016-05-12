//Thomas Mayo-Smith
//tmayosmi


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include "cachelab.h"


struct cacheLine{
    int tag;
    int LRUC;
    int valid;
};

int gettag(int addr,int s, int b)
{
    //leading 1's don't matter bc tag will be unique
    return (addr)>>(s+b);
}

int getset(int addr,int tag, int s, int b)
{
    //delete block bits
    int tagAndset= addr>>b;

    //when anded with tag and set only set will remain
    int mask = (1<<s)-1;

    return mask&tagAndset;
}


int main(int argc, char** argv)
{
    int opt,s, E, b;
    char* path;

    while(-1 != (opt = getopt(argc, argv,"s:E:b:t:")))
        switch(opt){
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                path = optarg;
                break;  
            default:
                printf("wrong argument\n"); 
                break;
        }

    FILE * pFile;
    pFile = fopen (path,"r");
    char identifier; 
    unsigned address; 
    int size;

    //memory for storing "line" structs
    struct cacheLine cache[1<<s][E];

    //counters...
    int misses=0;
    int hits=0; 
    int evictions=0;
    int set,tag;

    //line with largest "least recently used" counter
    int maxline;

    //explained later in code
    int freeline;
    int found=0;

    while(fscanf(pFile," %c %x,%d", &identifier, &address, &size)>0)
    { 
        if(identifier != 'I')
        {
            //get tag and set
            tag=gettag(address,s,b);
            set=getset(address,tag,s,b);

            //stays false after loop if tag wasn't found in set
            found=0;
            //stays at -1 if no free line is found in set
            freeline=-1;

            maxline=0; 

            //loop through set of address
            for(int i = 0; i<E; i++)
            {
                if((cache[set][i]).valid==1)
                {
                    if(((cache[set][i]).tag==tag))
                    {
                        found=1;
                        //reset most recently used
                        cache[set][i].LRUC=0;
                    }
                    else
                    {
                        //increment least recently used if not used by current
                        //instruction
                        cache[set][i].LRUC+=1;
                        //keep track of oldest cache element
                        if((cache[set][i]).LRUC > cache[set][maxline].LRUC)
                        {
                            maxline=i;
                        }
                    }
                }
                else 
                {
                    //i will not be -1 if there is a free line
                    freeline=i;
                }
            }

            
            if(found==1)
            {
                if(identifier=='L' || identifier=='S')
                {
                    hits+=1;
                }
                //identifier is M
                else
                {
                    //simulate "load" and "store" part of modify function
                    hits+=2;
                }
            }
            else
            {
                //for L/S/"load" part of M
                misses+=1;
                if(identifier=='M')
                {
                    //simulate "store" part of M
                    hits+=1;
                }
                if(freeline != -1)
                {
                    cache[set][freeline].LRUC=0;
                    cache[set][freeline].tag=tag;
                    cache[set][freeline].valid=1;
                }
                //evict
                else
                {
                    evictions+=1;
                    cache[set][maxline].LRUC=0;
                    cache[set][maxline].tag=tag;
                    cache[set][maxline].valid=1;
                }
            }
            //simulate "store" part of modify function
            //in the way it would increment "LRUC"
            if(identifier=='M')
            {
                for(int i = 0; i<E; i++)
                {
                    //an LRUC of 0 indicates this was the line previously loaded as aresult
                    //of the M function. Thus would have been the line whos "LRUC" would 
                    //have been set to zero had the "store" part of the M function been 
                    //carried out. Thus it is kept at zero.
                    if(cache[set][i].LRUC!=0)
                    {
                        cache[set][i].LRUC+=1;
                    }
                }
            }
        }
    }

    printSummary(hits, misses, evictions);

    fclose(pFile);
    
    return 0;
}

