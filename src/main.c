#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "lexer.h"


bool is_regular_file(char* path) {
    struct stat path_stat;
    stat(path, &path_stat);

    return S_ISREG(path_stat.st_mode);
}

int get_file_length(FILE* fp) {
    int length;

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return length;
}

int main(int argc, char* argv[]) {
   if(argc < 2) {
       printf("Must provide a filename.\n");
       exit(1);
   }

   FILE* fp = fopen(argv[1], "r");

   if(fp == NULL || !is_regular_file(argv[1])) {
       printf("File does not exist.\n");
       exit(1);
   }

   int length = get_file_length(fp);

   char* content = malloc(length);

   if(content == NULL) {
       printf("There was a problem allocating memory.\n");
       exit(1);
   }

   fread(content, length, 1, fp);

    
}
