#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
        int sockfd;
        char buffer[1000];
        char server_reply[2000];
        ssize_t n;
        struct sockaddr_in server_addr;

//CREATE SOCKET
        sockfd = socket(AF_INET , SOCK_STREAM , 0);
        if (sockfd == -1)
        {
                perror("SOCKET CANNOT BE CREATED!\n");
        }

        printf("SOCKET HAS BEEN CREATED!\n");

        bzero(&server_addr,sizeof (server_addr));
        server_addr.sin_addr.s_addr = inet_addr("192.168.56.20");
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(23);

//CONNECT TO REMOTE SERVER
                connect(sockfd, (struct sockaddr *)&server_addr,sizeof(server_addr));

//SEND SOME DATA
                while(1)
                {
                        printf("ENTER A MESSAGE: ");
                        scanf("%s",buffer);
                        if (send(sockfd,buffer,strlen(buffer),0) < 0)
                        {
                                printf("ERROR! PLEASE TRY AGAIN!\n");
                                return 1;
                        }

//RECEIVE A REPLY FROM A SERVER
                        if (recv(sockfd,server_reply,2000,0) < 0)
                        {
                                puts("ERROR! PLEASE TRY AGAIN!");
                                break;
                        }
                        printf("SERVER REPLY: %s \n",server_reply);
                }
}
