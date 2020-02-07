/*
 * Copyright (c) 2020, 4Embedded.Systems
 * All Rights Reserved.
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


/* ------- Defines --------------------------------------------------------- */


/* ------- Types ----------------------------------------------------------- */


/* ------- Local Routines -------------------------------------------------- */

//static char* interfaces_list = "eth0";

/* ------- Implementation -------------------------------------------------- */



int main(int argc, char *argv[])
{
      int simpleSocket = 0, simplePort = 0, returnStatus = 0, check = 1, n;
      char buffer[1024];

      struct addrinfo simpleServer, *res;

      if (2 != argc) {
          fprintf(stderr, "Usage: %s <port>\n", argv[0]);
          exit(1);
      }

      simplePort = atoi(argv[1]);

      memset(&simpleServer, 0, sizeof simpleServer);
      simpleServer.ai_family = AF_INET6;  // use IPv4 or IPv6, whichever
      simpleServer.ai_socktype = SOCK_STREAM;
      simpleServer.ai_flags = AI_PASSIVE | AI_V4MAPPED ;

      getaddrinfo(NULL, argv[1], &simpleServer, &res);

      simpleSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
      if (simpleSocket)
      {
        returnStatus = bind(simpleSocket, res->ai_addr, res->ai_addrlen);
        returnStatus = listen(simpleSocket, 5);

        struct addrinfo clientName = { 0 };
        socklen_t clientNameLength = sizeof(clientName);
        int simpleChildSocket = 0;

        while (1) {
           while (1) {

              fprintf(stdout,"Waiting..\n");

              simpleChildSocket = accept(simpleSocket,(struct sockaddr *)&clientName, &clientNameLength);
              if (simpleChildSocket)
              {
                fprintf(stdout,"Reading..\n");

                memset(&buffer, '\0', sizeof(buffer));
                returnStatus = read(simpleChildSocket, buffer, sizeof(buffer));

                fprintf(stdout, "Message: %s\n", buffer);

                write(simpleChildSocket, buffer, sizeof(buffer));

                close(simpleChildSocket);
              }
              else
              {
                fprintf(stdout, "simpleChildSocket - ERROR\n");
              }
           }
        }
        close(simpleSocket);
      }
      else
      {
        fprintf(stdout, "simpleSocket - ERROR\n");
      }
      return 0;
}
