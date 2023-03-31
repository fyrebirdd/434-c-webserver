#ifndef SERVER_H_
#define SERVER_H_

#define BACKLOG 10

struct web_server {
    int sockfd;
};

struct web_server create_web_server(char* hostname, char* port);


#endif