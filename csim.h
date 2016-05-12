#ifndef _CSIM_H_
#define _CSIM_H_

int gettag(int addr, int s, int b);
int getset(int addr,int tag, int s, int b);

typedef int bool;
#define true 1
#define false 0

#endif