#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Route{

    char* path;
    char* file;

    struct Route *next;
};

struct Route* init(char* path, char* file){
    struct Route* start = (struct Route*) malloc(sizeof(struct Route));

    start->path = path;
    start->file = file;

    start->next = NULL;

    return start;
}


struct Route* add(struct Route* r, char* path, char* file){
    if(strcmp(path, r->path) == 0){
        printf("ERROR: this tree already containts %s\n", path);
    }else if (r->next == NULL)
    {
        r->next = init(path, file);
    }else{
        add(r->next, path, file);
    }  
}
