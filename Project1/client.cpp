//------Include libraries and setting variablies------//
#include <iostream>
#include "unp.h"
#define _bufsize_ 1024
//----------------Error display function--------------//
void err_sy(const char* x) 
{ 
    perror(x); 
    exit(1); 
}
//----------------Main function-----------------------//
int main(int argc,char *argv[]){

    setbuf(stdout, NULL);
    if (argc != 3)
    {
        err_sy("Only two arguement shuold be given(IP and Port)\n");
    }
    // Setting variablies and structures
    char *ip = argv[1];
    char *port = argv[2];
    int server_port = atoi(port);
    char buf[_bufsize_];
    memset(buf,0,sizeof(buf));
    struct sockaddr_in server_sock;
    // Creating socket file descriptor
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        err_sy("socket error");
    }
    bzero(&server_sock, sizeof(server_sock));
    server_sock.sin_family = AF_INET;
    server_sock.sin_port = htons(server_port);
    //  Attachmenting the ip of the server to the structure
    int chi = inet_pton(AF_INET, ip , &server_sock.sin_addr);
    if (chi < 0){
        err_sy("inet_pton error");
    }
    // Connect to the server
    int ret = connect(sock, (struct sockaddr*)& server_sock, sizeof(server_sock));
    printf("Trying to connect...\n");
    if (ret < 0)
    {
        err_sy("connect error");
    }
    printf("connect sucess!\n");
    while(1){
        printf("Type your msg :");
	bool isEOR = false ;
        if(fgets(buf, sizeof(buf), stdin)!=NULL){

	    buf[strlen(buf)-1] = '\0';          
            write(sock, buf, sizeof(buf));

	}
	else{
	    isEOR = true ;
	}
        if (strncasecmp(buf,"quit", 4) == 0 or isEOR)
        {
            printf("quit !\n");
            break;
        }
        read(sock, buf, sizeof(buf));
	int s=strlen(buf);
        printf("Get echo msg from server as : %s\n", buf);
    }
    close(sock);
    return 0;
}
