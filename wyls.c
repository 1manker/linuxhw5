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


void getPerms(char *file){
    struct stat fileStat;
    
    if(stat(file,&fileStat) < 0){
        return;
    }
    else{
        printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
        printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
        printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
        printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
        printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
        printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
        printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
        printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
        printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
        printf((fileStat.st_mode & S_IXOTH) ? "x " : "- ");

    }
}

char *combStr(const char *chars1, char *chars2){
    char *ret;
    size_t len1 = strlen(chars1);
    size_t len2 = strlen(chars2);
    int i, j;
    ret = malloc((len1+len2+1)*sizeof *ret);
    strcpy(ret, chars1);
    for(i=len1, j=0; ((i<(len1+len2))&&(j<len2)); i++, j++){
        ret[i] = chars2[j];
    }
    ret[len1+len2] = '\0';
    return ret;
}

int main(int argc, char** argv){
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));
    DIR *d;
    if(argc > 1){
        d = opendir(argv[1]);
        sprintf(cwd, "%s", argv[1]);
    }
    else{
        d = opendir("./");
        sprintf(cwd,"./");
    }
    struct dirent *dir;
    if(d){
        while((dir=readdir(d)) != NULL){
            const char firstLetter = dir->d_name[0];
            char *perCh;
            perCh=strchr(&firstLetter,'.');
            size_t num = perCh-&firstLetter;
            if(num > 2){
                getPerms(combStr(cwd,dir->d_name));
                printf("%s\n", dir->d_name);
            }
        }
        closedir(d);
    }
    return 0;
}
