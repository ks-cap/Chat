
// client_chat.c

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>            /* 'atoi'を利用するために定義 */

#define SOCKET_ERROR        -1
#define BUFFER_SIZE         127
#define HOST_NAME_SIZE      255

int  main(int argc, char* argv[])
{
  int hSocket;                 /* handle to socket */
  struct hostent* pHostInfo;   /* holds info about a machine */
  struct sockaddr_in Address;  /* Internet socket address stuct */
  long nHostAddress;           /* ホストアドレス */
  char pBuffer[BUFFER_SIZE];   /* 文字列格納場所 */
  unsigned nReadAmount;        /* データの大きさを格納する変数*/
  char strHostName[HOST_NAME_SIZE];  /* ホストネーム */
  int nHostPort;               /* ポート番号を格納する変数 */

  
  if(argc < 3) {
    printf("\nUsage: client host-name host-port\n");
    return 0;
  } else {
    /* argv[1] の値を strHostName にコピー */
    strcpy(strHostName,argv[1]);
    /* 文字列で表現された数値をint型の数値に変換(ポート番号が格納) */
    nHostPort=atoi(argv[2]);
  }
  
  printf("\nMaking a socket"); /* make a socket */
  hSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(hSocket == SOCKET_ERROR) {
    printf("\nCould not make a socket\n");
    return 0;
  }
  
  pHostInfo=gethostbyname(strHostName);  /* get IP address from name */
  memcpy(&nHostAddress,pHostInfo->h_addr,pHostInfo->h_length); /* copy address into long */
  
  /* 変数 nHostAddress を in_addr_t型と定義 */
  Address.sin_addr.s_addr=(in_addr_t)nHostAddress;  /* fill address struct */
  Address.sin_port=htons(nHostPort);
  Address.sin_family=AF_INET;
  
  printf("\nConnecting to %s on port %d",strHostName,nHostPort);
  if(connect(hSocket,(struct sockaddr*)&Address,sizeof(Address)) == SOCKET_ERROR) { /* connect to host */
    printf("\nCould not connect to host\n");
    return 0;
  }
  
  nReadAmount=read(hSocket, pBuffer, BUFFER_SIZE); /* read from socket into buffer*/
  printf("\nReceived \"%s\" from server\n",pBuffer);
  
  write(hSocket, pBuffer, nReadAmount);  /* write what we received back to server */
  printf("\nWriting \"%s\" to server",pBuffer);
  
  
  printf("\nClosing socket\n"); /* close socket */
  if(close(hSocket) == SOCKET_ERROR) {
    printf("\nCould not close socket\n");
    return 0;
  }
}

