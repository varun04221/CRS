#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include "directory_utils.h"


int count(const char *filename,int req){
    FILE *ptr;
    char ch;
    ptr=fopen(filename,"r");
    if (ptr==NULL){
        printf("Something unexpected occured \n");
        return -1;
    }
    int spaces =0;
    int newline=0;
    bool last=false;
    bool last2=false;
    do {
        ch=fgetc(ptr);
        if (ch==' '){spaces++;if (last2){newline++;}last=true;}
        else if (ch=='\n' && last){last2=true;}
        else if (ch!=EOF){last=false;last2=false;}
    }while (ch!=EOF);

    if (last){spaces--;}

    fclose(ptr);
    if (req==0){
    return spaces+1;}
    else{return spaces+1-newline;}

}

int main(){
    while (true){
        char *input=NULL;
        size_t input_size=0;
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s:", cwd);
        } else {
            perror("getcwd() error. Terminating program.");
            break;
        }
        ssize_t chars_read =getline(&input,&input_size,stdin);

        if (chars_read>0 && input[chars_read-1]=='\n'){
            input[chars_read-1]='\0';
        }

        char *ele =strtok(input," ");
        char **arg=NULL;
        int len=0;

        while (ele !=NULL){
            arg =realloc(arg, (len+1)*sizeof(char *));
            arg[len]=ele;
            len++;

            ele=strtok(NULL, " ");
        }
        
    if (strcmp(arg[0],"word")==0){
        if (len==2){
            int ans=count(arg[1],0);
            if (ans!=-1){printf("No of words in the given file:%d\n",ans);}
        }
        else if (len==3){
            int ans=count(arg[2],1);
            if (ans!=-1){printf("No of words in the given file:%d\n",ans);}
            else{printf("Terminating the program.\n");break;}
        }
        else if (len==4){
            int ans1=count(arg[2],0);
            int ans2=count(arg[3],0);
            if (ans1==-1 || ans2==-1){printf("Terminating the program.\n");break;}
            else{printf("The difference between the number of words in the input file is:%d\n",abs(ans1-ans2));}
        }
        else{
            printf("Invalid command terminating the program \n");
            break;
        }
        }
    else if (strcmp(arg[0],"date")==0){
        pid_t pid;
        pid=fork();
        char* argv[]={arg[1],arg[2],NULL};
        if (pid==0){
            execvp("./dated",(char *const *) argv);
        }
        else if (pid>0){
            wait(NULL);
        }
        else{
            printf("Fork Failed. Terminating the program.\n");
            break;
        }
        }
    else if (strcmp(arg[0],"dir")==0 ){
        pid_t pid;
        pid=vfork();
        char* argv[]={arg[1],arg[2],NULL};
        if (pid==0){
        bool present=false;

        struct dirent* dir;
        DIR *directory =opendir(".");
        
        if (directory==NULL){perror("Failed");}
        while ((dir=readdir(directory))!=NULL){
            if (strcmp(dir->d_name,argv[1])==0){present=true;break;}
        }

        if (present){
            if (strcmp(argv[0],"-r")!=0){perror("Directory already Exist.");}
            else{deletedir(argv[1]);makedir(argv[1]);changedir(argv[1]);getcwd(cwd, sizeof(cwd));}
        }
        else{
            if (strcmp(argv[0],"-r")==0){makedir(argv[1]);changedir(argv[1]);getcwd(cwd, sizeof(cwd));}
            else{
                makedir(argv[1]);
                printf("Created the directory %s \n",argv[1]);
                changedir(argv[1]);
                getcwd(cwd, sizeof(cwd));
            }
        }
        }
        else if (pid>0){
            wait(NULL);
        }
        else{
            printf("Fork Failed. Terminating the program.\n");
            break;
        }
    }
    else{
        printf("Invalid Argument terminating the program \n");
        break;
        }
    }
    return 0;
}