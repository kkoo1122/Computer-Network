//------Include libraries and setting variablies------//
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
    if (argc != 2)
    {
        err_sy("Only one arguement shuold be given(Port)\n");
    }
    // Setting variablies and structures
    char *port = argv[1];
    char buff[MAXLINE];
    char buf_ip[INET_ADDRSTRLEN];
    int _PORT_ = atoi(port);
    struct sockaddr_in servaddr;
    struct sockaddr_in clientaddr;
    
    // Creating socket file descriptor
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if ( sock < 0 ){
	err_sy("socket error");
    }
    memset(buf_ip, 0, sizeof(buf_ip));
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = (INADDR_ANY);
    servaddr.sin_port = htons(_PORT_);

    // Attaching socket to the port
    if ( bind(sock, (struct sockaddr *) &servaddr, sizeof(servaddr) ) < 0){
	err_sy("bind error");
    }
    // Setting maximum accpetable clients
    if ( listen(sock, 10) < 0){
	err_sy("listen error");
    }
    while (1){
        socklen_t len = 64 ;
        printf("Server is waiting for connections... \n");
        int client_addr =  accept(sock, (struct sockaddr *) &clientaddr, &len) ; 
	inet_ntop(AF_INET, &clientaddr.sin_addr, buf_ip, sizeof(buf_ip));
	printf("Server get connection from : %s, with port : %d\n",buf_ip, ntohs(clientaddr.sin_port) );
	if ( client_addr < 0 ){
	     err_sy("client error");
	     close(sock);	
	}
	pid_t id = fork();
        if (id<0){
            err_sy("fork");
        }
	else if (id==0){
	    close(sock);
	    pid_t idd = fork();

  	    if(idd<0){
	    	printf("second fork");
	    }
	    else if(idd==0){
		while(1){
 		    char buf[_bufsize_];
		    memset(buf, 0, sizeof(buf));
                    bool isEOR = false ;
		    if (read(client_addr, buf, sizeof(buf))){
                        printf("Get msg from %s : %s\n",buf_ip, buf); 
		        write(client_addr, buf, strlen(buf)+1);
		    }
		    else{
			isEOR = true ;
		    }
        	    if (strncasecmp(buf,"quit", 4) == 0 or isEOR)
        	    {
           		printf("client disconnection from : %s \n",buf_ip);
            	   	close(client_addr);
			break;
        	    }		
		}
		exit(0);
	    }
	    else{
		exit(0);
  	    }
	}		
	else{
	    close(client_addr);
	    waitpid(id,NULL,0);
	}
    }
    close(sock);
    return 0;
}
