/*
 * Copyright (c) 2020, 4Embedded.Systems
 * All Rights Reserved.
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


/* ------- Defines --------------------------------------------------------- */
#define BUF_SIZE      500

#define GET_FAMILY_NAME(_family) (      \
 (_family) == AF_PACKET ? "AF_PACKET" : \
 (_family) == AF_INET   ? "AF_INET"   : \
 (_family) == AF_INET6  ? "AF_INET6"  : \
                          "???"         \
)

/* ------- Types ----------------------------------------------------------- */
typedef struct n6c_if_net_s
{
  char*         name;
  unsigned int  flags;
  sa_family_t   family;

  struct n6c_if_net_s* next;

} n6c_if_net_t, *n6c_if_net_p;


/* ------- Local Routines -------------------------------------------------- */

static char n6c_buffer[BUF_SIZE] = {0};
static char text_buf[500];

static n6c_if_net_p    n6c_if_net_list_ptr = NULL;
static n6c_if_net_p    n6c_if_net_last_ptr = NULL;


/* ------- Implementation -------------------------------------------------- */

static n6c_if_net_p n6cAddNetIf(char* name, unsigned int flags, sa_family_t family)
{
  n6c_if_net_p if_ptr = (n6c_if_net_p) malloc(sizeof(n6c_if_net_t));

  printf("++%s(name: %s, flags: 0x%08x, family: %d [%s])\n", __FUNCTION__, name ? name : "---", flags, family, GET_FAMILY_NAME(family));
  if (if_ptr != NULL)
  {
    if_ptr->name = NULL;
    if (name)
    {
      if_ptr->name = (char*) malloc(strlen(name) + 1);
      if (if_ptr->name)
      {
        strcpy(if_ptr->name, name);
      }
    }
    if_ptr->flags   = flags;
    if_ptr->family  = family;
    if_ptr->next    = NULL;

    if (n6c_if_net_list_ptr == NULL)
    {
      n6c_if_net_list_ptr = if_ptr;
    }
    else
    {
      n6c_if_net_last_ptr->next = if_ptr;
    }
    n6c_if_net_last_ptr = if_ptr;
  }
  printf("--%s()\n", __FUNCTION__);
  return if_ptr;
}

static void n6cFreeNetIfList(void)
{
  n6c_if_net_p if_ptr = n6c_if_net_list_ptr;

  printf("++%s()\n", __FUNCTION__);
  while (if_ptr != NULL)
  {
    n6c_if_net_p del_ptr = if_ptr;

    if_ptr = del_ptr->next;

    if (del_ptr->name)
    {
      free(del_ptr->name);
    }
    free(del_ptr);
  }
  n6c_if_net_list_ptr = NULL;
  n6c_if_net_last_ptr = NULL;
  printf("--%s()\n", __FUNCTION__);
}

static n6c_if_net_p n6cFindNextNetIf(sa_family_t family, n6c_if_net_p if_ptr)
{
  printf("++%s(family: %d [%s], if_ptr: %p)\n", __FUNCTION__, family, GET_FAMILY_NAME(family), if_ptr);
  if (if_ptr == NULL)
  {
    if_ptr = n6c_if_net_list_ptr;
  }
  else
  {
    if_ptr = if_ptr->next;
  }

  while (if_ptr != NULL)
  {
    if ((family == PF_UNSPEC) || (if_ptr->family == family))
    {
      break;
    }
    if_ptr = if_ptr->next;
  }
  printf("--%s() - if_ptr: %p\n", __FUNCTION__, if_ptr);
  return if_ptr;
}


static char* n6cGetFlags2Name(unsigned int flags)
{

  text_buf[0] = 0;
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_UP          ? "UP, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_BROADCAST   ? "BROADCAST, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_DEBUG       ? "DEBUG, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_LOOPBACK    ? "LOOPBACK, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_POINTOPOINT ? "POINTOPOINT, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_NOTRAILERS  ? "NOTRAILERS, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_RUNNING     ? "RUNNING, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_NOARP       ? "NOARP, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_PROMISC     ? "PROMISC, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_ALLMULTI    ? "ALLMULTI, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_MASTER      ? "MASTER, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_SLAVE       ? "SLAVE, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_MULTICAST   ? "MULTICAST, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_PORTSEL     ? "PORTSEL, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_AUTOMEDIA   ? "AUTOMEDIA, " : "");
  sprintf(text_buf, "%s%s", text_buf, flags & IFF_DYNAMIC     ? "DYNAMIC, " : "");
  return text_buf;
}

static void n6cGetNetIfList(void)
{
  struct ifaddrs *ifaddr, *ifa;

  printf("++%s()\n", __FUNCTION__);
  if (getifaddrs(&ifaddr) != -1)
  {
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
      if (ifa->ifa_addr != NULL)
      {
        printf("%-8s   [0x%08x] %s\n\r\t\t%s (%d)\n",
               ifa->ifa_name,
               ifa->ifa_flags, n6cGetFlags2Name(ifa->ifa_flags),
               GET_FAMILY_NAME(ifa->ifa_addr->sa_family), ifa->ifa_addr->sa_family);

        if ((ifa->ifa_addr->sa_family == AF_INET) || (ifa->ifa_addr->sa_family == AF_INET6))
        {
          if ((ifa->ifa_flags & IFF_LOOPBACK) == 0)
          {
            n6cAddNetIf(ifa->ifa_name, ifa->ifa_flags, ifa->ifa_addr->sa_family);
          }
        }
      }
    }
  }
  printf("--%s()\n", __FUNCTION__);
}

int main(int argc, char *argv[])
{
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int sfd, s, j;
  size_t len;
  ssize_t nread;
  char buf[BUF_SIZE];

  n6c_if_net_p if_ptr = NULL;

  if (argc < 3) {
     fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
     exit(EXIT_FAILURE);
  }
#if 0
  n6cGetNetIfList();


  while ((if_ptr = n6cFindNextNetIf(AF_INET6, if_ptr)) != NULL)
  {
    printf("[%p] -> %s\n", if_ptr, if_ptr->name ? if_ptr->name : "---");
  }

  n6cFreeNetIfList();
#endif

#if 1

  /* Obtain address(es) matching host/port */

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_NUMERICSERV | AI_NUMERICHOST | AI_V4MAPPED | AI_ALL;
  hints.ai_protocol = 0;          /* Any protocol */

  s = getaddrinfo(argv[1], argv[2], &hints, &result);
  if (s != 0) {
     fprintf(stderr, "getaddrinfo: %d=%s\n", s, gai_strerror(s));
     exit(EXIT_FAILURE);
  }

  /* getaddrinfo() returns a list of address structures.
    Try each address until we successfully connect(2).
    If socket(2) (or connect(2)) fails, we (close the socket
    and) try the next address. */

  for (rp = result; rp != NULL; rp = rp->ai_next)
  {
   unsigned char* addr_ptr = (unsigned char*) rp->ai_addr;

   printf("[%p] -> socket(ai_family: %d [%s], ai_socktype: %d, ai_protocol: %d), ai_addrlen: %d\n", rp,
               rp->ai_family, GET_FAMILY_NAME(rp->ai_family), rp->ai_socktype, rp->ai_protocol, rp->ai_addrlen);

   printf("[%p]      +--> len: %d\n", rp, rp->ai_addrlen);
   printf("[%p]      +--> addr: ", rp);
   for (int i = 0; i < rp->ai_addrlen; i++)
   {
     printf("%02x ", addr_ptr[i]);
     //if ((i > 0) && (i % 2))
   }
   printf("\n");

   sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
   printf("[%p] -> socket() - sfd: %d\n", rp, sfd);
   if (sfd == -1)
       continue;

   printf("[%p] -> connect(sfd: %d)\n", rp, sfd);

   if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
   {
       printf("[%p] -> connect(sfd: %d) - OK\n", rp, sfd);
       break;                  /* Success */
   }

   close(sfd);

  }

  if (rp == NULL) {               /* No address succeeded */
     fprintf(stderr, "Could not connect\n");
     exit(EXIT_FAILURE);
  }

  freeaddrinfo(result);           /* No longer needed */

  /* Send remaining command-line arguments as separate
    datagrams, and read responses from server */

  for (j = 3; j < argc; j++) {
     len = strlen(argv[j]) + 1;
             /* +1 for terminating null byte */

     if (len + 1 > BUF_SIZE) {
         fprintf(stderr,
                 "Ignoring long message in argument %d\n", j);
         continue;
     }

     if (write(sfd, argv[j], len) != len) {
         fprintf(stderr, "partial/failed write\n");
         exit(EXIT_FAILURE);
     }

     nread = read(sfd, buf, BUF_SIZE);
     if (nread == -1) {
         perror("read");
         exit(EXIT_FAILURE);
     }

     printf("Received %zd bytes: %s\n", nread, buf);
  }
#endif
  exit(EXIT_SUCCESS);
}

