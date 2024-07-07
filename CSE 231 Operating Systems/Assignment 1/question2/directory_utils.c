#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

void deletedir(const char *path){
    struct dirent *dir;
    DIR *directory=opendir(path);

    if (directory==NULL){perror("something is broken");return;}
    while ((dir=readdir(directory))!=NULL){
        if (strcmp(dir->d_name,".")==0 || strcmp(dir->d_name,"..")==0){continue;}
        char fpath[PATH_MAX];
        snprintf(fpath,sizeof(fpath),"%s/%s",path,dir->d_name);
        if (dir->d_type == DT_DIR){
            deletedir(fpath);
        }
        else{
            remove(fpath);
        }
    }
    closedir(directory);
    rmdir(path);
    return;
}

void makedir(const char *name){
    int apple=mkdir(name,0755);
    if (apple != 0){perror("Error Creating the directory!");}
    return;
}

void changedir(const char *name){
    int apple=chdir(name);
    if (apple!=0){perror("Error Changing the directory");return;}
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        
    } else {
            perror("getcwd() error. Terminating program.");}
    return;
}
