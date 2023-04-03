#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "routes.h"


struct Route* init(char* path, char* file){
    struct Route* start = (struct Route*) malloc(sizeof(struct Route));

    start->path = path;
    start->file = file;

    start->next = NULL;

    return start;
}


void add(struct Route* r, char* path, char* file){
    if(strcmp(path, r->path) == 0){
        printf("ERROR: this tree already contains %s\n", path);
    }else if (r->next == NULL)
    {
        r->next = init(path, file);
    }else{
        printf("added %s", path);
        add(r->next, path, file);
    }
}

struct Route* get(struct Route* r, char* path){
    
    if(strcmp(path, r->path) == 0){
        return r;
    }
    else if (r->next == NULL){
        printf("ERROR: this tree does not contain %s\n", path);
        return NULL;
    }else{
        return get(r->next, path);
    }
}
