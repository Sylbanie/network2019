#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>


int main(int argc, char **argv) {
    struct hostent *hp;
    if (argc < 2) {
      char hostName[200];
      printf("Enter hostname: ");
      fgets(hostName, sizeof(hostName), stdin);
      hp = gethostbyname(hostName);
    }
    else {
      hp = gethostbyname(argv[1]);
    }

    if (hp == NULL) {
       printf("gethostbyname() failed\n");
    } else {
       printf("%s = ", hp->h_name);
       unsigned int i=0;
       while ( hp -> h_addr_list[i] != NULL) {
          printf( "%s ", inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[i])));
          i++;
       }
       printf("\n");
    }

    int sockfd;
    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("Error creating socket\n");
      exit(1);
    }
  
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    memcpy((char *) &saddr.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);
    saddr.sin_port = htons(8785);
    if (connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
        printf("Cannot connect\n");
        exit(1);
    }
    printf("Successfully connected to server\n");

    while (1) {
      char s[200];
      memset(s, 0, 200);
      printf("Client> ");
      fgets(s, 200, stdin);
      s[strlen(s) - 1] = 0;
      if (strcmp(s, "/quit") == 0) {
        shutdown(sockfd, SHUT_RDWR);
        close(sockfd);
        exit(0);
      }
      write(sockfd, s, strlen(s));

      if (read(sockfd, s, 200) <= 0) {
        printf("Server disconnected.\n");
        break;
      }
      printf("Server says: %s\n", s);
    }
}