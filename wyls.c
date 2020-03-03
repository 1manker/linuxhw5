/* 
 * wyls.c
 * Lucas Manker
 * 2/27/20
 * COSC 3750 Homework 5
 *
 * simple program which mimics ls
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>

void humanRead(long size){
    double kb = size /1024.00;
    if(kb < 1024.00){
        printf("%*.1fK ",6, kb);
        return;
    }
    double mb = kb / 1024.00;
    if(mb < 1024.00){
        printf("%*.1fM ",6,mb);
        return;
    }
    double gb = mb /1024.00;
    printf("%*.1fG ",6,gb);

    return;
}

void getPerms(char *file, int* flags){
    struct stat fileStat;
    if(stat(file,&fileStat) < 0){
        return;
    }
    else{
        struct passwd *pw = getpwuid(fileStat.st_uid);
        struct group *gr = getgrgid(fileStat.st_gid);
        char date [100];
        long dif = time(0) - fileStat.st_mtime;
        if(dif > 15552000){
            strftime(date, 20, "%b %d %Y ", localtime(&(fileStat.st_mtime)));
        }
        else{
            strftime(date, 20, "%b %d %H:%M", localtime(&(fileStat.st_mtime)));
        }


        printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
        printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
        printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
        printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
        printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
        printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
        printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
        printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
        printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
        printf((fileStat.st_mode & S_IXOTH) ? "x  " : "-  ");
        if(flags[0] == 1){
            printf("%*d ",5,fileStat.st_uid);
            printf("%*d ",5, fileStat.st_gid);
        }
        else{
            printf("%*s ",5, pw->pw_name);
            printf("%s  ", gr->gr_name);
        }
        if(flags[1] == 1){
            humanRead(fileStat.st_size);
        }
        else{
            printf("%*lld ",8, fileStat.st_size);
            printf("%s  ", date);
        }

    }
}

void processDir(DIR *d, char* cwd, int* flags){
    struct dirent *dir;
    if(d){
        while((dir=readdir(d)) != NULL){
            const char firstLetter = dir->d_name[0];
            char *perCh;
            perCh=strchr(&firstLetter,'.');
            size_t num = perCh-&firstLetter;
            if(num > 2){
                char buff[100];
                strcpy(buff, cwd);
                strcat(buff,dir->d_name);
                getPerms(buff, flags);
                printf("%s\n", dir->d_name);
            }
        }
        closedir(d);
    }
}

int main(int argc, char** argv){
    int flags[2];
    flags[0] = 0;
    flags[1] = 0;
    if(argc > 1){
        int i; 
        for(i = 1; i < argc; i++){
            const char first = argv[1][0];
            char *perCh;
            perCh=strchr(&first, '-');
            size_t num = perCh-&first;
            if(num < 2 && i == 1){
                int j;
                for(j = 1; j < strlen(argv[1]); j++){
                    const char next = argv[1][j];
                    char *peChN;
                    peChN=strchr(&next, 'n');
                    size_t nSize = peChN-&next;
                    char *peChH;
                    peChH=strchr(&next, 'h');
                    size_t hSize = peChH-&next;
                    if(nSize < 2){
                        flags[0] = 1;
                    }
                    else if(hSize < 2){
                        flags[1] = 1;
                    }
                    else{
                        printf("%s: command not found\n", &next);
                        exit(1);
                    }
                }
            }
            else{
                char cwd[4096];
                getcwd(cwd, sizeof(cwd));
                DIR *d;
                d = opendir(argv[i]);
                sprintf(cwd, "%s", argv[i]);
                printf("\n%s:\n", cwd);
                processDir(d,cwd,flags);
            }
        }
    }
    if(argc == 2 && (flags[0] == 1 || flags[1] == 1)){
        char cwd[4096];
        getcwd(cwd, sizeof(cwd));
        DIR *d;
        d = opendir("./");
        sprintf(cwd, "./");
        processDir(d,cwd,flags);
    }
    if(argc == 1){
        char cwd[4096];
        getcwd(cwd, sizeof(cwd));
        DIR *d;
        d = opendir("./");
        sprintf(cwd,"./");
        processDir(d,cwd,flags);
    }
    return 0;
}
