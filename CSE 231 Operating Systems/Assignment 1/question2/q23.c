#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

int main(int argc,char* argv[]){
    const char *filename=argv[argc-1];
    struct stat fileinfo;

    if (stat(filename,&fileinfo) == 0){
        time_t mtime=fileinfo.st_mtime;
        struct tm *timeinfo = localtime(&mtime);

        
    if (strcmp(argv[0],"-d")==0){
        char str[256];
        if (strcmp(argv[1],"yesterday")==0){
            timeinfo->tm_mday--;
            if (timeinfo->tm_mday<=0){
                timeinfo->tm_mon--;
                if (timeinfo->tm_mon<0){
                    timeinfo->tm_mon=11;
                    timeinfo->tm_year--;
                }

                struct tm last_month=*timeinfo;
                last_month.tm_mday=1;
                mktime(&last_month);
                timeinfo->tm_mday=last_month.tm_mday;
            }
            strftime(str,sizeof(str),"%a, %d %b %Y %H:%M:%S",timeinfo);
            printf("%s",str);
        }
        else{
        strftime(str,sizeof(str),"%a, %d %b %Y %H:%M:%S",timeinfo);
        printf("%s \n",str);
        }
    }
    else if (strcmp(argv[0],"-R")==0){
        char rfc5322[256];
        strftime(rfc5322,sizeof(rfc5322),"%a, %d %b %Y %H:%M:%S %z",timeinfo);
        printf("%s \n",rfc5322);
    }
    else{
        printf("Invalid Argument\n");
    }
    }
    else{
        perror("Something unexpected happened");
    }
    return 0;
}