#include <stdlib.h>
#include <string.h>

struct Route{

    char* path;
    char* file;

    struct Route *next;
};

struct Route* init(char* path, char* file);

void add(struct Route* r, char* path, char* file);

struct Route* get(struct Route* r, char* path);