#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "server.h"


char *parse_html_file(char* file_name){
    FILE* f ;
    if ((f= fopen(file_name, "r")) == NULL){
        printf("File not found\n");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *parsed_html = (char *)malloc(sizeof(char) * (file_size+1));
    char tempChar;
    int i = 0;
    while ((tempChar = fgetc(f)) != EOF){
        parsed_html[i] = tempChar;
        i++;
    }
    parsed_html[i] = '\0';
    fclose(f);
    return parsed_html;
}


struct web_server create_web_server(char* hostname, char* port){
    struct web_server ws;
    int status;
    struct addrinfo hints, *res, *p;
    int yes=1;

    memset (&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(hostname, port, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
    }   

    for(p = res; p != NULL; p = p->ai_next){
        if ((ws.sockfd= socket(p->ai_family, p->ai_socktype, 
        p->ai_protocol)) == -1){
            perror("server: socket");
            continue;
        }//if socket fails to create

        if (setsockopt(ws.sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
        sizeof(int)) == -1){
            perror("setsockopt");
            exit(1);
        }//if it fails to set the socket option

        if (bind(ws.sockfd, p->ai_addr, p->ai_addrlen) == -1){
            close(ws.sockfd);
            perror("server: bind");
            continue;
        }//if it fails to bind to the given port

        break;
    }

    freeaddrinfo(res);
    if (p == NULL){
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }//couldnt find place to create the socket

    if (listen(ws.sockfd, BACKLOG) == -1){
        perror("listen");
        exit(1);
    }//if it fails to start listening on the socket
    printf("Server initialized on http://%s/%s\n", hostname, port);
    printf("Waiting for connections....\n");
    return ws;
}

int main(int argc, char const *argv[])
{

    
    if (argc != 2){
        printf("Proper Usage: ./server PORT");
        exit(1);
    }//if not enough arguments were entered
    
    char port[6];
    strcpy(port,argv[1]);
    
    struct web_server ws;

    ws = create_web_server("localhost", port);


    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    int incoming_socket_fd;

    for(;;){

        char msg[4096] = "";

        sin_size = sizeof their_addr;
        incoming_socket_fd = accept(ws.sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (incoming_socket_fd == -1){
            perror("accept");
            continue;
        }// if accept errored

        read(incoming_socket_fd, msg, 4095);
        printf("%s\n", msg);//print incoming http request

        char *response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
        char *html_page = parse_html_file("index.html"); //TEMP, CHANGE THIS WHEN IMPLEMENTING ROUTING

        write(incoming_socket_fd, response, strlen(response));
        write(incoming_socket_fd, html_page, strlen(html_page));
        close(incoming_socket_fd);
    }
    return 0;
}
