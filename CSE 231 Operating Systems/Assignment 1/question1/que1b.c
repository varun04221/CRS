#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(int argc,char *argv[]){
    int ch=fork();
    if (ch<0){printf("fork failed!");}
    else if (ch==0){
        int f4=1;
        for (int i=1;i<5;i++){f4*=i;}
        printf("The factorial of 4 is %d\n",f4);
    }
    else{
        wait(NULL);
        int series[16];series[0]=1;series[1]=1;
        for (int i=2;i<16;i++){series[i]=series[i-1]+series[i-2];}
        printf("The fibonacci series: ");
        for (int i=0;i<16;i++){printf("%d ",series[i]);}
        printf("\n");
    }

    return 0;
}