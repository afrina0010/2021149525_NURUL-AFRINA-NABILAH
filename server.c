#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>


int main()
{
        void *server (void *telnet);


        int listenfd, connfd, *new_sock;
        socklen_t ssize;
        struct sockaddr_in client_addr, server_addr;

//LISTEN
        listenfd = socket(AF_INET,SOCK_STREAM,0);

//CREATE SOCKET
        if(listenfd == -1)
        {
                perror("SOCKET CANNOT BE CREATED \n");
        }
        puts("SOCKET HAS BEEN CREATED SUCCESSFULLY!");

        bzero(&server_addr,sizeof (server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(23);

//BINDING
        if (bind(listenfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
        {
                perror("BIND FAILED");
                return 1;
        }
        puts("BIND SUCCESS");
  
        listen(listenfd,5);

        puts("WAITING..");
        ssize = sizeof(client_addr);

  while ((connfd = accept(listenfd,(struct sockaddr *) &client_addr, &ssize)))
        {
                puts("SUCCESSFULLY CONNECTED");

                pthread_t server_thread;
                new_sock = malloc(1);
                *new_sock = connfd;
                pthread_create(&server_thread,NULL,server,(void*)new_sock);
        }

        if (connfd < 0)
        {
                perror("FAILED");
                return 1;
        }

        return 0;
}

//FUNCTION TO READ THE CLIENT MEESAGE
void *server (void *telnet)
{
        printf("HYE SERVER HANDLER \n");

        int sock = *(int *) telnet;
        int read_size, write_size;
        char *message;
        static char client_message[2000];
        message = "\n HYE SERVER HANDLER\n";

        static int message_sent = 0;
        if(message_sent <= 0)
        {
                message_sent++;
        }

        while((read_size = recv(sock,client_message,2000,0))>0)
        {
                printf("READ SIZE %d \n",read_size);
                write(sock,client_message,strlen(client_message));
        }

        if(read_size == 0)
        {
                puts("CLIENT HAS BEEN DISCONNECTED!");
                fflush(stdout);
        }
        else if (read_size == -1)
        {
                perror("FAILED TO RECEIVE THE MESSAGE :(");
        }
        free (telnet);

        close(sock);
        return 0;
}
