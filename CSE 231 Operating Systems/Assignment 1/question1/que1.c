#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
    printf("Program starts here.\n");
    int ch=fork();
    if (ch<0){printf("fork failed!");}
    else if (ch==0){
        sleep(1);
        printf("Child is having ID:%d \n",(int)getpid());
        printf("My parent's id is:%lld \n",(long long)getppid());
        }
    else{
        printf("Parent (P) is having ID:%d \n",(int)getpid());
        wait(NULL);
        printf("ID of P's Child is %d \n",ch);
    }
    return 0;
}