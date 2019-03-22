#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

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

}